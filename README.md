# VaultCLI: A C-Based Password Manager

VaultCLI is a lightweight command-line interface application written in C for managing and generating passwords. It features local binary file storage and uses XOR obfuscation to prevent credentials from being stored in plain text.

---

## Features

* **Encrypted Storage**: Automatically obfuscates service names, usernames, and passwords using XOR bitwise operations before saving to a binary file.
* **Duplicate Prevention**: Scans the existing database to check for matching service names before allowing a new entry.
* **Credential Search**: Allows users to locate specific account details by searching for the service name.
* **Strength Evaluation**: Analyzes passwords based on length and the presence of uppercase letters, lowercase letters, digits, and special characters.
* **Password Generation**: Creates randomized 20-character passwords following a specific complexity pattern.

---

## Technical Implementation

* **File I/O**: Utilizes `fopen`, `fwrite`, and `fread` to manage persistent data in `passwordVault.bin`.
* **Data Structures**: Uses a custom `struct` to organize service, username, and password data into fixed-size blocks.
* **Memory and Logic**: Employs `rewind` and `getc` to calculate file size and handle stored entries.
* **Input Handling**: Implements buffer clearing with `getchar` to prevent input errors during menu navigation.

---

## Navigation
1. **Add new password**: Input the service, username, and password to be encrypted and saved.
2. **View all saved passwords**: Decrypts and displays every entry currently in the vault.
3. **Search for a password**: Find a specific entry by its service name using case-insensitive matching.
4. **Check strength of password**: Provides a rating of a password's security based on character variety and length.
5. **Generate strong password**: Generate multiple high-complexity 20-character passwords.

---

## Security Disclaimer
This project was developed for educational purposes to demonstrate file handling and basic data obfuscation in C. The XOR encryption used is a symmetric cipher intended for learning; it is a form of obfuscation and does not provide the high-level security required for protecting sensitive real-world production data.
