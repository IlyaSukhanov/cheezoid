#!/bin/bash -e

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
PI_HOSTNAME="${1:-cheezoid-jr}"
SSH_DEST="pi@${PI_HOSTNAME}"

NC='\033[0m'  # No Color
TEAL='\033[0;36m'

function log() {
  local msg=${1}
  echo -e "\n${TEAL} @@@@@@ ${1}${NC}\n"
}

function copy_files() {
  log "Deploying ${DIR} to ${PI_HOSTNAME}..."
  ssh -n ${SSH_DEST} "rm -rf ~/cheezoid/"
  rsync -v -r --exclude=".*" --exclude=".*/" "${DIR}" ${SSH_DEST}:.
}

function restart_services() {
  log "Restarting services..."
  echo "killall..."
  ssh -n ${SSH_DEST} "killall python || true"
  for service in cheesed httpd; do
    echo "start ${service}..."
    ssh -n ${SSH_DEST} "sh -c 'cd ~/cheezoid/${service} && nohup python run-${service}.py > /tmp/${service}.log 2>&1 < /dev/null &'"
  done
}

function main() {
  copy_files
  restart_services
}

main
log "Finished"
