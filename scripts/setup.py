#
# This file is a part of tls-client implementation for
# modern C++ (17+ standard)
#
# Thanks for bogdanfinn for creating the original tls-client
# library in GO https://github.com/bogdanfinn/tls-client
# 

import os
import sys
import shutil
import requests

from urllib.parse import urljoin

class Dependencies:
    EXPECTED_FILES = [
        "tls-client-windows-32-1.7.5.dll",     
        "tls-client-windows-64-1.7.5.dll",      
        "tls-client-xgo-1.7.5-linux-amd64.so",  
        "tls-client-xgo-1.7.5-linux-arm64.so",      
        "tls-client-xgo-1.7.5-linux-386.so",        
        "tls-client-xgo-1.7.5-darwin-amd64.dylib",  
        "tls-client-xgo-1.7.5-darwin-arm64.dylib"   
    ]

    @staticmethod
    def download_dependencies() -> None:
        folder_path = 'dependencies/'
        url_base = 'https://github.com/bogdanfinn/tls-client/releases/download/v1.7.5/'

        if not os.path.isdir(folder_path):
            os.makedirs(folder_path)

        print(f"Downloading missing files to '{folder_path}'...")

        for file in Dependencies.EXPECTED_FILES:
            file_path = os.path.join(folder_path, file)

            if os.path.isfile(file_path):
                print(f"File already exists: {file}")
                continue

            url = urljoin(url_base, file)
            response = requests.get(url, stream=True)

            if response.status_code == 200:
                with open(file_path, 'wb') as f:
                    shutil.copyfileobj(response.raw, f)
                print(f"Downloaded: {file}")
            else:
                print(f"Failed to download: {file} ({response.status_code})")

    @staticmethod
    def find_missing_files(folder_path: str) -> list:
        if not os.path.isdir(folder_path):
            return Dependencies.EXPECTED_FILES 

        files_in_folder = os.listdir(folder_path)
        missing_files = [
            file 
            for file in Dependencies.EXPECTED_FILES 
            if file not in files_in_folder
        ]
        return missing_files
    
    @staticmethod
    def check_dependencies() -> None:
        folder_path = 'dependencies'
        
        if not os.path.isdir(folder_path):
            print(f"Folder '{folder_path}' not found.")
            try:
                os.makedirs(folder_path)
                print(f"Created folder '{folder_path}'.")
            except:
                print(f"Failed to create folder '{folder_path}'.")
                sys.exit(0)
            
            Dependencies.check_dependencies()  
            return  

        print(f"Found folder '{folder_path}'. Checking for required files...")
        
        missing_files = Dependencies.find_missing_files(folder_path)
        
        if missing_files:
            print("Some files are missing. Downloading...")
            Dependencies.download_dependencies()
        else:
            print("All required files are present!")

class Docs:
    @staticmethod 
    def ask_for_docs() -> bool:
        while True:
            create_docs = False

            try:
                _create_docs = input("Create docs? (yes/no): ")
            except:
                return False
            
            if _create_docs == "yes":
                return True

            return create_docs
        
    @staticmethod
    def create_docs() -> None:
        if not Docs.ask_for_docs():
            print("Skipping building tests")
            return
        
        import subprocess
        
        try:
            subprocess.run(["doxygen"], check=True)
            print("Doxygen documentation created successfully.")
        except Exception as e:
            print(f"Error creating Doxygen documentation: {e}")

def main():
    Dependencies.check_dependencies()  
    Docs.create_docs()

if __name__ == '__main__':
    main()