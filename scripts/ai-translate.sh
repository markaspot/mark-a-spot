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
      "taxonomy_service_categories.csv"|"taxonomy_service_provider.csv"|"taxonomy_service_status.csv"|"page.csv"|"boilerplate.csv"|"block.csv"|"menu.csv"|"group_jurisdiction.csv"|"group_organisation.csv")
        # Continue with translation
        ;;
      *)
        echo "  Skipping translation for $filename, just updating language code..."
        # Copy header
        head -1 "$csv_file" > "$output_file"
        # Just update language code for skipped files that have a langcode column
        if grep -q "langcode" "$csv_file"; then
             tail -n +2 "$csv_file" | sed "s/\"en\"/\"$LANG_CODE\"/g" >> "$output_file"
        else
             tail -n +2 "$csv_file" >> "$output_file"
        fi
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

    # --- Prompt Generation (Identical to your original script) ---
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
        echo "IMPORTANT: Translate this CSV data from English to $LANG_CODE. The CSV has THREE columns: 'uuid', 'title', and 'body'. Translate ONLY the 'title' and 'body' columns. The 'uuid' column MUST remain EXACTLY the same. Maintain all HTML formatting in the body column. Return ONLY the translated CSV rows without the header. Format each row exactly like the original with all commas, quotes and HTML tags preserved." > "$prompt_file"
        echo "" >> "$prompt_file"
        echo "IMPORTANT TRANSLATION REQUIREMENTS:" >> "$prompt_file"
        echo "1. Each row has format: \"UUID\",\"TITLE\",\"BODY HTML CONTENT\"" >> "$prompt_file"
        echo "2. You MUST translate ONLY the TITLE and BODY columns" >> "$prompt_file"
        echo "3. Do NOT change the UUID - it must be preserved exactly" >> "$prompt_file"
        echo "4. Example input: \"a1b2c3d4-1111-4000-8000-000000000001\",\"Imprint\",\"<h2>Lorem ipsum...\"" >> "$prompt_file"
        echo "5. Example output for German: \"a1b2c3d4-1111-4000-8000-000000000001\",\"Impressum\",\"<h2>Lorem ipsum (translated)...\"" >> "$prompt_file"
        echo "6. All titles MUST be translated to $LANG_CODE" >> "$prompt_file"
        ;;
      "boilerplate.csv")
        echo "IMPORTANT: Translate this CSV data from English to $LANG_CODE. The CSV has THREE columns: 'uuid', 'title', and 'body'. Translate ONLY the 'title' and 'body' columns. The 'uuid' column MUST remain EXACTLY the same. Maintain all HTML formatting in the body column. Return ONLY the translated CSV rows without the header. Format each row exactly like the original with all commas, quotes and HTML tags preserved." > "$prompt_file"
        echo "" >> "$prompt_file"
        echo "IMPORTANT TRANSLATION REQUIREMENTS:" >> "$prompt_file"
        echo "1. Each row has format: \"UUID\",\"TITLE\",\"BODY CONTENT\"" >> "$prompt_file"
        echo "2. You MUST translate ONLY the TITLE and BODY columns" >> "$prompt_file"
        echo "3. Do NOT change the UUID - it must be preserved exactly" >> "$prompt_file"
        echo "4. Example input: \"b1b2c3d4-0001-4000-8000-000000000001\",\"001 Thank you\",\"Lorem ipsum...\"" >> "$prompt_file"
        echo "5. Example output for German: \"b1b2c3d4-0001-4000-8000-000000000001\",\"001 Danke\",\"Lorem ipsum (translated)...\"" >> "$prompt_file"
        echo "6. All titles MUST be translated to $LANG_CODE" >> "$prompt_file"
        ;;
      "block.csv")
        echo "IMPORTANT: Translate this CSV data from English to $LANG_CODE. The CSV has four columns: 'title', 'uuid', 'body', and 'body_format'. Translate ONLY the 'title' and 'body' columns. DO NOT translate the 'uuid' and 'body_format' columns. Maintain all HTML formatting in the body column. Return ONLY the translated CSV rows without the header. Format each row exactly like the original with all commas, quotes and HTML tags preserved." > "$prompt_file"
        echo "" >> "$prompt_file"
        echo "IMPORTANT TRANSLATION REQUIREMENTS:" >> "$prompt_file"
        echo "1. Each row has format: \"TITLE\",\"UUID\",\"BODY HTML CONTENT\",\"BODY_FORMAT\"" >> "$prompt_file"
        echo "2. You MUST translate ONLY the TITLE and BODY columns" >> "$prompt_file"
        echo "3. Do NOT translate the UUID and BODY_FORMAT columns" >> "$prompt_file"
        echo "4. Example input: \"Block Title\",\"c049959f-cc6e-18806-a118-9959fcc6eb97\",\"<p>Block content...</p>\",\"full_html\"" >> "$prompt_file"
        echo "5. Example output for German: \"Block Titel\",\"c049959f-cc6e-18806-a118-9959fcc6eb97\",\"<p>Block Inhalt...</p>\",\"full_html\"" >> "$prompt_file"
        echo "6. All titles MUST be translated to $LANG_CODE" >> "$prompt_file"
        ;;
       "menu.csv")
         echo "Translate this CSV data from English to $LANG_CODE. Translate ONLY the text in column 1 (title) and column 2 (description). Change langcode in column 4 from \"en\" to \"$LANG_CODE\". All other columns MUST remain EXACTLY the same, without any changes to numbers, formatting, quotes or values. This is critically important for all fields, especially uuid, link.uri, weight and parent. Return ONLY the translated CSV rows without the header." > "$prompt_file"
         ;;
      "group_jurisdiction.csv"|"group_organisation.csv")
        echo "Translate this CSV data from English to $LANG_CODE. The CSV has columns: uuid, langcode, type, label, field_head_organisation_e_mail. Translate ONLY the 'label' column (column 4). Change langcode in column 2 from \"en\" to \"$LANG_CODE\". All other columns MUST remain EXACTLY the same - especially uuid, type, and email addresses. Return ONLY the translated CSV rows without the header." > "$prompt_file"
        echo "" >> "$prompt_file"
        echo "IMPORTANT:" >> "$prompt_file"
        echo "1. Only translate the label (e.g., 'Default Jurisdiction' -> 'Jurisdicción Predeterminada' for Spanish)" >> "$prompt_file"
        echo "2. Keep uuid exactly as-is (e.g., 550e8400-e29b-41d4-a716-446655440001)" >> "$prompt_file"
        echo "3. Keep type exactly as-is (e.g., 'jur' or 'org')" >> "$prompt_file"
        echo "4. Keep email addresses exactly as-is" >> "$prompt_file"
        ;;
    esac
    # --- End Prompt Generation ---

    # Add header and content to prompt
    echo "" >> "$prompt_file"
    echo "CSV HEADER (for reference only, do not return this):" >> "$prompt_file"
    echo "$header" >> "$prompt_file"
    echo "" >> "$prompt_file"
    echo "CONTENT TO TRANSLATE:" >> "$prompt_file"
    cat "$content_file" >> "$prompt_file"

    # Create the JSON request file
    request_file="${TRANSLATED_DIR}/request_${filename}.json"

    # Choose the model - gpt-4o-mini is fast and cost-effective for translation
    MODEL=${OPENAI_MODEL:-"gpt-4o-mini"}
    echo "  Using model: $MODEL"

    jq -n --arg system "You are a CSV translator. You will translate specified columns in CSV data while preserving all formatting, quotes, commas, and other data exactly. Your output must be valid CSV format with the same structure as the input. Do not include CSV header in the output. Do not add markdown code fences like \`\`\`csv or \`\`\` around the output." \
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
    if jq -e '.error' "$response_file" > /dev/null; then # More robust error check
      echo "  ERROR: OpenAI API returned an error:"
      jq -r '.error.message' "$response_file" 2>/dev/null || cat "$response_file"
      echo "  Using original file with updated language code instead..."
      # Copy header
      head -1 "$csv_file" > "$output_file"
      # Just update language code (if applicable) for the file
      if grep -q "langcode" "$csv_file"; then
          tail -n +2 "$csv_file" | sed "s/\"en\"/\"$LANG_CODE\"/g" >> "$output_file"
      else
          tail -n +2 "$csv_file" >> "$output_file"
      fi
      continue
    fi

    # Extract the raw translated content using jq
    translated_raw_file="${TRANSLATED_DIR}/translated_raw_${filename}.txt"
    jq -r '.choices[0].message.content' "$response_file" > "$translated_raw_file"

    # --- START: Robust Markdown Cleanup and Validation ---
    translated_cleaned_file="${TRANSLATED_DIR}/translated_cleaned_${filename}.csv"

    # Check if we got markdown code blocks and clean if necessary
    if grep -q '^```' "$translated_raw_file"; then
      echo "  Detected markdown format, extracting CSV content..."
      # Extract content between markdown code blocks and remove any leading/trailing blank lines
      sed -n '/^```/,/^```/ { /^```/d; p; }' "$translated_raw_file" | grep . > "$translated_cleaned_file"
    else
      # No fences detected, just remove any leading/trailing blank lines
      grep . "$translated_raw_file" > "$translated_cleaned_file"
    fi

    # Validate the *cleaned* translated content against original content (ignoring blank lines)
    # Count non-empty lines in the original content
    original_content_lines=$(tail -n +2 "$csv_file" | grep . | wc -l)
    # Count non-empty lines in the cleaned translated content
    translated_content_lines=$(wc -l < "$translated_cleaned_file") # grep already removed blank lines

    echo "  Original non-empty content lines: $original_content_lines"
    echo "  Translated non-empty content lines: $translated_content_lines"

    if [ -s "$translated_cleaned_file" ] && [ "$translated_content_lines" -eq "$original_content_lines" ]; then
      # Validation passed! Write the final output file.
      echo "$header" > "$output_file"
      cat "$translated_cleaned_file" >> "$output_file"
      echo "  Successfully translated and validated file ($translated_content_lines lines)"

    else
      # Validation failed! Use original content with updated lang code as fallback.
      echo "  Warning: Translation validation failed (line count mismatch: got $translated_content_lines, expected $original_content_lines, or empty file)"
      echo "  Using original content with updated language code as fallback."
      echo "$header" > "$output_file"
      if grep -q "langcode" "$csv_file"; then
          tail -n +2 "$csv_file" | sed "s/\"en\"/\"$LANG_CODE\"/g" >> "$output_file"
      else
          tail -n +2 "$csv_file" >> "$output_file"
      fi
    fi
    # --- END: Robust Markdown Cleanup and Validation ---


  fi
done # End of file loop

# Translated files stay in $TRANSLATED_DIR - do NOT copy back to main artifacts
# The original English CSVs remain untouched for base content import
# Translations will be added via create-translations.php from $TRANSLATED_DIR

echo "Translated CSVs created in: $TRANSLATED_DIR"
echo "Original English CSVs in $ARTIFACTS_DIR remain unchanged."

# Clean up the language-specific temporary files (keep the final CSVs)
echo "Cleaning up temporary translation files..."
rm -f "$TRANSLATED_DIR"/prompt_*.txt
rm -f "$TRANSLATED_DIR"/content_*.txt
rm -f "$TRANSLATED_DIR"/request_*.json
rm -f "$TRANSLATED_DIR"/response_*.json
rm -f "$TRANSLATED_DIR"/translated_raw_*.txt
rm -f "$TRANSLATED_DIR"/translated_cleaned_*.csv
# Keep the final results in $TRANSLATED_DIR/*.csv as a record

echo ""
echo "--- Translation Script Finished ---"
echo "Translated CSVs ready in: $TRANSLATED_DIR"
echo "Use 'drush php:script scripts/create-translations.php -- $LANG_CODE' to add translations"