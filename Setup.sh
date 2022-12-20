#!/bin/bash
set -e

sudo apt update -y
sudo apt install ninja-build -y

echo ""
echo "Setup complete."
echo ""

exit 0