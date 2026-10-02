#!/usr/bin/env bash
set -euo pipefail
binary=$1

expect() {
    if [[ "$output" != *"$1"* ]]; then
        printf 'Missing CLI output: %s\n%s\n' "$1" "$output" >&2
        exit 1
    fi
}

preview_count() {
    local line
    line=$(printf '%s\n' "$output" | grep '^Legal actions:' | head -n 1)
    local count
    count=$(printf '%s\n' "$line" | grep -o 'n:' | wc -l)
    [[ "$count" -eq "$1" ]]
}

output=$(printf 'bad\n4\nq\n' | "$binary")
expect 'Choose 1, 2, 3, or q.'
expect 'Goodbye.'
output=$("$binary" </dev/null)
expect 'Goodbye.'

for game in 1 2 3; do
    output=$(printf '%s\nwrong\n0 0 extra\n2147483648 0\n99 99\nq\n' "$game" | "$binary")
    expect 'Input:'
    expect 'You move first.'
    expect 'Invalid input:'
    expect 'Illegal move:'
    expect 'Goodbye.'
    case "$game" in
        1) expect 'row col, both 0..2'; preview_count 9 ;;
        2) expect 'row col, both 0..10'; expect '111 more actions available.'; preview_count 10 ;;
        3) expect 'Input: q r'; expect '51 more actions available.'; preview_count 10 ;;
    esac
    output=$(printf '%s\n' "$game" | "$binary")
    expect 'Goodbye.'
done

output=$(printf '1\n0 0\n0 0\nq\n' | "$binary")
expect 'AI move:'
expect 'Illegal move:'
expect 'Goodbye.'
printf 'CLI smoke checks passed.\n'