#!/bin/sh

# CSV file translator using OpenAI API
echo "Starting AI translation script"

# Check if language argument is provided
if [ -z "$1" ]; then
  echo "Usage: ai-translate.sh <language-code> [api-key]"
  echo "Example: ai-translate.sh de"
  exit 1
fi

# Set variables
LANG_CODE="$1"
API_KEY=${2:-$OPENAI_API_KEY}

# Check if API key is available
if [ -z "$API_KEY" ]; then
  echo "Error: OpenAI API key is required."
  exit 1
fi

# Find the artifacts directory
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
ARTIFACTS_DIR="$PROJECT_ROOT/web/profiles/contrib/markaspot/modules/markaspot_default_content/artifacts"
if [ ! -d "$ARTIFACTS_DIR" ]; then
  echo "Error: Could not find artifacts directory at $ARTIFACTS_DIR"
  exit 1
fi

# Create translated directory
TRANSLATED_DIR="${ARTIFACTS_DIR}/${LANG_CODE}"
mkdir -p "$TRANSLATED_DIR"
echo "Created translation directory at $TRANSLATED_DIR"

# Process each CSV file
for csv_file in "$ARTIFACTS_DIR"/*.csv; do
  if [ -f "$csv_file" ]; then
    filename=$(basename "$csv_file")
    output_file="$TRANSLATED_DIR/$filename"
    echo "Translating $filename to $LANG_CODE..."
    
    # Skip files that don't need translation or are empty
    # Skip if file is empty (just has header or no content)
    if [ ! -s "$csv_file" ] || [ $(wc -l < "$csv_file") -le 1 ]; then
      echo "  Skipping empty file $filename..."
      cp "$csv_file" "$output_file"
      continue
    fi
    
    case "$filename" in
      "taxonomy_service_categories.csv"|"taxonomy_service_provider.csv"|"taxonomy_service_status.csv"|"page.csv"|"boilerplate.csv"|"block.csv"|"menu.csv")
        # Continue with translation
        ;;
      *)
        echo "  Skipping translation for $filename, just updating language code..."
        # Copy header
        head -1 "$csv_file" > "$output_file"
        # Just update language code for skipped files
        tail -n +2 "$csv_file" | sed 's/"en"/"'"$LANG_CODE"'"/g' >> "$output_file"
        continue
        ;;
    esac
    
    # Get header and content
    header=$(head -1 "$csv_file")
    content=$(tail -n +2 "$csv_file")
    
    # Create a temporary file with the content
    content_file="${TRANSLATED_DIR}/content_${filename}.txt"
    echo "$content" > "$content_file"
    
    # Create the prompt file (simple text file)
    prompt_file="${TRANSLATED_DIR}/prompt_${filename}.txt"
    
    # Write the prompt based on file type
    case "$filename" in
      "taxonomy_service_categories.csv")
        echo "Translate this CSV data from English to $LANG_CODE. Translate ONLY the text in column 4 (name) and column 5 (description). Change langcode in column 3 from \"en\" to \"$LANG_CODE\". All other columns MUST remain EXACTLY the same, without any changes to numbers, formatting, quotes or values. This is critically important for fields: vid, tid, weight, parent, uuid, field_category_gid, field_category_hex, field_category_hex_opacity, field_category_icon, field_service_code. Return ONLY the translated CSV rows without the header." > "$prompt_file"
        ;;
      "taxonomy_service_provider.csv")
        echo "Translate this CSV data from English to $LANG_CODE. Translate ONLY the text in column 4 (name) and column 5 (description). Change langcode in column 3 from \"en\" to \"$LANG_CODE\". All other columns MUST remain EXACTLY the same, without any changes to numbers, formatting, quotes or values. This is critically important for all fields, especially email addresses. Return ONLY the translated CSV rows without the header." > "$prompt_file"
        ;;
      "taxonomy_service_status.csv")
        echo "Translate this CSV data from English to $LANG_CODE. Translate ONLY the text in column 4 (name) and column 5 (description). Change langcode in column 3 from \"en\" to \"$LANG_CODE\". All other columns MUST remain EXACTLY the same, without any changes to numbers, formatting, quotes or values. This is critically important for all fields, especially hex color codes and icon names. Return ONLY the translated CSV rows without the header." > "$prompt_file"
        ;;
      "page.csv")
        echo "Translate this CSV data from English to $LANG_CODE. You MUST preserve the exact column structure. The CSV has only two columns: 'title' and 'body'. Both columns MUST be translated to $LANG_CODE while maintaining all HTML formatting. Return ONLY the translated CSV rows without the header. Format each row exactly like the original with all commas, quotes and HTML tags preserved." > "$prompt_file"
        ;;
      "boilerplate.csv")
        echo "Translate this CSV data from English to $LANG_CODE. You MUST preserve the exact column structure. The CSV has only two columns: 'title' and 'body'. Both columns MUST be translated to $LANG_CODE while maintaining all HTML formatting. Return ONLY the translated CSV rows without the header. Format each row exactly like the original with all commas, quotes and HTML tags preserved." > "$prompt_file"
        ;;
      "block.csv")
        echo "Translate this CSV data from English to $LANG_CODE. You MUST preserve the exact column structure. The CSV has only two columns: 'title' and 'body'. Both columns MUST be translated to $LANG_CODE while maintaining all HTML formatting. Return ONLY the translated CSV rows without the header. Format each row exactly like the original with all commas, quotes and HTML tags preserved." > "$prompt_file"
        ;;
      "menu.csv")
        echo "Translate this CSV data from English to $LANG_CODE. If the file is empty or only has a header row, just copy it as is. If it has content, translate ONLY column 4 (menu title) and change langcode in column 3 from \"en\" to \"$LANG_CODE\". All other columns MUST remain EXACTLY the same, without any changes to numbers, formatting, quotes or values. Return ONLY the translated CSV rows without the header." > "$prompt_file"
        ;;
    esac
    
    # Add header and content to prompt
    echo "" >> "$prompt_file"
    echo "CSV HEADER (for reference only, do not return this):" >> "$prompt_file"
    echo "$header" >> "$prompt_file"
    echo "" >> "$prompt_file"
    echo "CONTENT TO TRANSLATE:" >> "$prompt_file"
    cat "$content_file" >> "$prompt_file"
    
    # Create the JSON request file
    request_file="${TRANSLATED_DIR}/request_${filename}.json"
    
    # Create the JSON payload using jq (much more reliable)
    # Choose the model - gpt-4o is preferred for better translation quality
    MODEL=${OPENAI_MODEL:-"gpt-4o"}
    echo "  Using model: $MODEL"
    
    jq -n --arg system "You are a CSV translator. You will translate specified columns in CSV data while preserving all formatting, quotes, commas, and other data exactly. Your output must be valid CSV format with the same structure as the input." \
         --arg user "$(cat "$prompt_file")" \
         --arg model "$MODEL" \
         '{
           model: $model,
           messages: [
             {role: "system", content: $system},
             {role: "user", content: $user}
           ]
         }' > "$request_file"
    
    # Make the API call
    response_file="${TRANSLATED_DIR}/response_${filename}.json"
    echo "  Calling OpenAI API..."
    curl -s https://api.openai.com/v1/chat/completions \
         -H "Content-Type: application/json; charset=utf-8" \
         -H "Authorization: Bearer $API_KEY" \
         --data-binary @"$request_file" > "$response_file"
    
    # Check for API errors
    if grep -q "error" "$response_file"; then
      echo "  ERROR: OpenAI API returned an error:"
      cat "$response_file" | jq '.error.message' 2>/dev/null || cat "$response_file"
      echo "  Using original file with updated language code instead..."
      # Copy header
      head -1 "$csv_file" > "$output_file"
      # Just update language code for the file
      tail -n +2 "$csv_file" | sed 's/"en"/"'"$LANG_CODE"'"/g' >> "$output_file"
      continue
    fi
    
    # Extract the translated content using jq (much more reliable)
    translated_file="${TRANSLATED_DIR}/translated_${filename}.csv"
    jq -r '.choices[0].message.content' "$response_file" > "$translated_file"
    
    # Special debug for page.csv
    if [ "$filename" = "page.csv" ]; then
      echo "  DEBUG: Checking API response for page.csv..."
      jq -r '.choices[0].message.content' "$response_file" | head -5
    fi
    
    # Check if we got markdown code blocks
    if grep -q '```' "$translated_file"; then
      echo "  Detected markdown format, extracting CSV content..."
      # Extract content between markdown code blocks
      sed -n '/^```/,/^```/ { /^```/d; p; }' "$translated_file" > "${translated_file}.tmp"
      mv "${translated_file}.tmp" "$translated_file"
    fi
    
    # Validate the translated CSV content
    translated_line_count=$(wc -l < "$translated_file")
    original_line_count=$(expr $(wc -l < "$csv_file") - 1)  # Subtract 1 for header
    
    if [ -s "$translated_file" ] && [ "$translated_line_count" -eq "$original_line_count" ]; then
      # Write the output file
      echo "$header" > "$output_file"
      cat "$translated_file" >> "$output_file"
      echo "  Successfully translated file ($translated_line_count lines)"
      
      # Debug output for page.csv
      if [ "$filename" = "page.csv" ]; then
        echo "  DEBUG: First few lines of translated page.csv:"
        head -3 "$output_file"
      fi
    else
      echo "  Warning: Translation failed or line count mismatch (got $translated_line_count, expected $original_line_count)"
      echo "  Using original with updated language code"
      echo "$header" > "$output_file"
      tail -n +2 "$csv_file" | sed 's/"en"/"'"$LANG_CODE"'"/g' >> "$output_file"
    fi
  fi
done

# Copy the translated files to the main artifacts directory for migration
# This ensures they'll be found by the migration process
echo "Copying translated files to the main artifacts directory..."
for translated_file in "$TRANSLATED_DIR"/*.csv; do
  if [ -f "$translated_file" ]; then
    filename=$(basename "$translated_file")
    # Back up the original file if it exists
    if [ -f "$ARTIFACTS_DIR/$filename" ]; then
      cp "$ARTIFACTS_DIR/$filename" "$ARTIFACTS_DIR/${filename}.bak"
      echo "  Backed up original $filename"
    fi
    # Copy the translated file to the main directory
    cp "$translated_file" "$ARTIFACTS_DIR/$filename"
    echo "  Copied translated $filename for migration"
  fi
done

# Modify the translated files to use the chosen language as a base language (not a translation)
# This ensures content is created directly in the chosen language instead of as translations
echo "Setting translated content as primary language content..."
for csv_file in "$ARTIFACTS_DIR"/*.csv; do
  if [ -f "$csv_file" ]; then
    # Skip files that don't have a langcode column
    if grep -q "langcode" "$csv_file"; then
      # Replace all instances of the language code with "en"
      # This makes Drupal treat the content as original content in the primary language
      sed -i "s/\"$LANG_CODE\"/\"en\"/g" "$csv_file"
      echo "  Updated language code in $(basename "$csv_file")"
    fi
  fi
done

# Clean up the language-specific directory
echo "Cleaning up temporary translation files..."
rm -f "$TRANSLATED_DIR"/prompt_*.txt
rm -f "$TRANSLATED_DIR"/content_*.txt
rm -f "$TRANSLATED_DIR"/request_*.json
rm -f "$TRANSLATED_DIR"/response_*.json
rm -f "$TRANSLATED_DIR"/translated_*.csv

echo "All translations completed successfully!"
echo "Translated files have been copied to $ARTIFACTS_DIR for migration"
echo "NOTE: Language-specific files are still available at: $TRANSLATED_DIR if needed"