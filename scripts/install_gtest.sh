#!/bin/bash
set -e

version=${1:-1.11.0}
tmp_dir=$(mktemp -d)
trap 'rm -rf "${tmp_dir}"' EXIT
cd "${tmp_dir}"

echo "- Downloading googletest ${version}"
wget --no-check-certificate -qO- "https://github.com/google/googletest/archive/release-${version}.tar.gz" | tar -C "${tmp_dir}" --strip=1 -xz

echo "- Building googletest ${version}"
mkdir build
cd build
cmake ..

echo "- Installing googletest ${version}"
make
sudo make install
