import os
import shutil

# Define paths
SOURCE_CONFIG_FILE = "config/config.h-template"
DESTINATION_CONFIG_FILE = "config/config.h"


def copy_config_file():
    # Copy file
    shutil.copy(SOURCE_CONFIG_FILE, DESTINATION_CONFIG_FILE)
    print(f"{SOURCE_CONFIG_FILE} has been copied to {DESTINATION_CONFIG_FILE}.")


# ------------------------ setup configuration ------------------------
# Check if the file already exists
if os.path.exists(DESTINATION_CONFIG_FILE):
    user_input = input(f"{DESTINATION_CONFIG_FILE} already exists. Do you want to overwrite it? [y/n]: ")
    if user_input.lower() != 'y':
        print(f"Copy {SOURCE_CONFIG_FILE} to {DESTINATION_CONFIG_FILE} aborted.")
    else:
        copy_config_file()
else:
    copy_config_file()
