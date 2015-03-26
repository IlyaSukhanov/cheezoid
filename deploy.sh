#!/bin/bash -e

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
PI_HOSTNAME="${1:-cheezoid-jr}"

echo "Deploying ${DIR} to ${PI_HOSTNAME}..."

rsync -v --size-only -r "${DIR}" pi@${PI_HOSTNAME}:.
