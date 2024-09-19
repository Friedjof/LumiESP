import os
import shutil

# Define paths
SOURCE_FILE = "config/config.h-template"
DESTINATION_FILE = "config/config.h"


# ------------------------ setup configuration ------------------------
# Check if the file already exists
if os.path.exists(DESTINATION_FILE):
    user_input = input(f"{DESTINATION_FILE} already exists. Do you want to overwrite it? [y/n]: ")
    if user_input.lower() != 'y':
        print("Copy operation aborted.")
        exit()
else:
    # Copy file
    shutil.copy(SOURCE_FILE, DESTINATION_FILE)
    print(f"{SOURCE_FILE} has been copied to {DESTINATION_FILE}.")
