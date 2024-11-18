#!/bin/bash
# Autor: Jan Jurasz
# Funkcja do wyświetlania pomocy
usage() {
    echo "Usage: $0 [options] DIRNAME"
    echo "Options:"
    echo "  --replace-with-hardlinks  Replace duplicates with hardlinks"
    echo "  --max-depth=N             Set max depth for directory traversal"
    echo "  --hash-algo=ALGO          Hash algorithm (md5sum, sha1sum, sha256sum)"
    echo "  --help                    Show this help message"
    exit 1
}

# Czy program podany w --hash-algo jest dostępny
check_hash_algo() {
    if ! command -v "$1" &>/dev/null; then
        echo "$1 not supported"
        exit 1
    fi
}

#Przeszukujemy katalog
find_files() {
    local dir=$1
    local max_depth=$2
    find "$dir" -type f -maxdepth "$max_depth"
}

# Funkcja do obliczania hasza
get_hash() {
    local file=$1
    local algo=$2
    "$algo" "$file" | awk '{ print $1 }'
}


compare_files() {
    local file1=$1
    local file2=$2
    cmp -s "$file1" "$file2"
}

# Tworzenie hardlink
create_hardlink() {
    local src=$1
    local dest=$2
    ln "$src" "$dest" || echo "Unable to create hardlink for $src -> $dest" >&2
}

# Główna funkcja 
process_directory() {
    local dir=$1
    local max_depth=$2
    local hash_algo=$3
    local replace_with_hardlinks=$4
    local interactive=$5

    if ! [[ "$max_depth" =~ ^[0-9]+$ ]]; then
        echo "Błąd: --max-depth musi być liczbą całkowitą"
        exit 1
    fi

    # Sprawdzamy dostępność algorytmu haszowania
    check_hash_algo "$hash_algo"

    if [[ ! -d "$dir" ]]; then
        echo "Błąd: Katalog '$dir' nie istnieje lub jest niedostępny"
        exit 1
    fi

    # Zbieramy  wszystkie pliki
    files=$(find_files "$dir" "$max_depth")

    declare -A file_hashes

    # Iterujemy po plikach
    for file in $files; do
        if [[ -f "$file" ]]; then
            size=$(stat --format="%s" "$file") || { echo "Cannot get size of file '$file'"; continue; }
        else
            echo "Skipped: file '$file' does not exist or is unavailable for some reason."
            continue
        fi

        hash=$(get_hash "$file" "$hash_algo")

        # Grupowanie plików po rozmiarze i hash
        if [[ -z "${file_hashes[$size,$hash]}" ]]; then
            file_hashes[$size,$hash]="$file"
        else
            file_hashes[$size,$hash]="${file_hashes[$size,$hash]} $file"
        fi
    done

    # Zmienne do raportu
    processed=0
    duplicates=0
    replaced=0

    # Przetwarzanie duplikatów
    for files_group in "${file_hashes[@]}"; do
        files_array=($files_group)
        if [ ${#files_array[@]} -gt 1 ]; then
            duplicates=$((duplicates + ${#files_array[@]} - 1))
            processed=$((processed + ${#files_array[@]}))

            # Jeśli opcja --replace-with-hardlinks jest włączona
            if [ "$replace_with_hardlinks" == "true" ]; then
                # Pierwszy plik jest oryginałem, reszta to duplikaty
                original_file=${files_array[0]}
                for duplicate in "${files_array[@]:1}"; do
                    if [ "$interactive" == "true" ]; then
                        read -p "Replace $duplicate with hardlink to $original_file? (y/n): " choice
                        if [[ "$choice" == "y" ]]; then
                            create_hardlink "$original_file" "$duplicate"
                            replaced=$((replaced + 1))
                        fi
                    else
                        create_hardlink "$original_file" "$duplicate"
                        replaced=$((replaced + 1))
                    fi
                done
            fi
        else
            processed=$((processed + 1))
        fi
    done

    # Raport
    echo "Liczba przetworzonych plików: $processed"
    echo "Liczba znalezionych duplikatów: $duplicates"
    echo "Liczba zastąpionych duplikatów: $replaced"

}

# Parsowanie argumentów 
replace_with_hardlinks=false
max_depth=999
hash_algo="md5sum"
interactive=false

while [[ $# -gt 0 ]]; do
    case "$1" in
        --replace-with-hardlinks)
            replace_with_hardlinks=true
            ;;
        --max-depth=*)
            max_depth="${1#*=}"
            ;;
        --hash-algo=*)
            hash_algo="${1#*=}"
            ;;
        --help)
            usage
            ;;
        *)
            dir=$1
            ;;
    esac
    shift
done

if [[ -z "$dir" ]]; then
    echo "Error: Directory name is required"
    usage
fi

# Uruchomienie przetwarzania
process_directory "$dir" "$max_depth" "$hash_algo" "$replace_with_hardlinks" "$interactive"
