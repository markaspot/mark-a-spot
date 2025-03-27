#!/bin/sh

# Check if input might be a full locale and extract language code
if [ -z "$1" ]; then
    echo "No language supplied, defaulting to english (en)"
    language="en"
    full_locale="en_US"
else
    # Check if input contains an underscore (like de_DE)
    if echo "$1" | grep -q "_"; then
        language=$(echo "$1" | cut -d '_' -f1)
        full_locale="$1"
        printf "\e[36mDetected full locale format (%s), using language code (%s) for translations\e[0m\n" "$full_locale" "$language"
    else
        language="$1"
        full_locale="${language}_${language^^}"
        printf "\e[36mUsing language code (%s) for translations\e[0m\n" "$language"
    fi
fi

# Set the translations directory
translations_dir="$(dirname "$0")/../translations"
mkdir -p "$translations_dir"

# Determine the Drupal version using the correct drush command
drupal_version=""
if drush core:status drupal-version --format=string >/dev/null 2>&1; then
    drupal_version=$(drush core:status drupal-version --format=string | cut -d. -f1-2)
elif drush status --field=drupal-version >/dev/null 2>&1; then
    drupal_version=$(drush status --field=drupal-version | cut -d. -f1-2)
else
    # Fallback to hardcoded version if commands fail
    printf "\e[33mWarning: Unable to determine Drupal version automatically. Assuming Drupal 11.0.\e[0m\n"
    drupal_version="11.0"
fi

printf "\e[36mImporting translation for $language (Drupal $drupal_version)...\e[0m\n"
drush language-add $language

# Check if we have a translation file
printf "\e[36mLooking for translation file in $translations_dir\e[0m\n"

# Function to download translation from drupal.org
download_translation() {
    local lang=$1
    local drupal_ver=$2
    local output_dir=$3
    local filename="drupal-$drupal_ver.$lang.po"
    local url="https://ftp.drupal.org/files/translations/$drupal_ver.x/drupal/drupal-$drupal_ver.x.$lang.po"
    
    printf "\e[36mDownloading translation from $url\e[0m\n"
    if command -v curl >/dev/null 2>&1; then
        curl -s -o "$output_dir/$filename" "$url"
    elif command -v wget >/dev/null 2>&1; then
        wget -q -O "$output_dir/$filename" "$url"
    else
        printf "\e[31mError: Neither curl nor wget found. Cannot download translation.\e[0m\n"
        return 1
    fi
    
    # Check if download was successful
    if [ -s "$output_dir/$filename" ]; then
        printf "\e[32mSuccessfully downloaded $filename\e[0m\n"
        return 0
    else
        rm -f "$output_dir/$filename"
        printf "\e[33mWarning: Could not download translation for $lang\e[0m\n"
        return 1
    fi
}

# Check for translation file in different formats
if [ -f "$translations_dir/$language.po" ]; then
    printf "\e[36mFound translation file: $language.po\e[0m\n"
    drush locale:import $language "$translations_dir/$language.po" --override=all
elif [ -f "$translations_dir/drupal-$drupal_version.$language.po" ]; then
    printf "\e[36mFound translation file: drupal-$drupal_version.$language.po\e[0m\n"
    drush locale:import $language "$translations_dir/drupal-$drupal_version.$language.po" --override=all
else
    printf "\e[33mNo local translation file found for $language, attempting to download...\e[0m\n"
    
    # Try to download the translation
    if download_translation "$language" "$drupal_version" "$translations_dir"; then
        drush locale:import $language "$translations_dir/drupal-$drupal_version.$language.po" --override=all
    else
        # Try direct import without downloading
        printf "\e[36mTrying to import translation directly via Drush...\e[0m\n"
        if drush locale:check && drush locale:update; then
            printf "\e[32mTranslations updated via Drush\e[0m\n"
        else
            printf "\e[33mWarning: Could not obtain translation for '$language'.\e[0m\n"
            printf "\e[33mYou may need to download translation manually from https://localize.drupal.org\e[0m\n"
        fi
    fi
fi

# Set the working directory to the script's parent directory
cd "$(dirname "$0")/.."

# Set configuration for the language
printf "\e[36mConfiguring language settings...\e[0m\n"
if [ -n "$full_locale" ]; then
    printf "\e[36mSetting language configuration for locale: $full_locale\e[0m\n"
    # Some configurations might benefit from full locale information
    drush config:set language.negotiation.selected_langcode $language -y 2>/dev/null || true
fi

drush config:set language.types negotiation.language_interface.enabled.$language 1 -y
drush config:set language.types negotiation.language_interface.method_id language-browser -y

# Check if language.administration config exists before trying to set it
if drush config:status language.administration >/dev/null 2>&1; then
  drush config:set language.administration negotiation.language_administration_language $language -y
else
  printf "\e[33mNote: language.administration config does not exist, skipping this setting\e[0m\n"
fi

# Use user:update instead of user:modify (both syntaxes for compatibility)
drush user:update 1 --langcode=$language 2>/dev/null || drush user-edit 1 --langcode=$language 2>/dev/null || printf "\e[33mCouldn't set user language preference, continuing...\e[0m\n"

# Set site default language
printf "\e[36mSetting site default language to $language\e[0m\n"
drush config:set system.site default_langcode $language -y

# Enable multilingual support
printf "\e[36mEnabling translatable Taxonomy Terms for multilingual Georeport Services...\e[0m\n"
drush en markaspot_language
drush cr

# Import additional translations if available
printf "\e[36mChecking for contributed module translations...\e[0m\n"
drush locale:update
