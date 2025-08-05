#!/usr/bin/env bash
set -euo pipefail

nix develop

clang -Wall -Wextra -Werror -DUSE_GUI -DUSE_NEWTON_SIMULATION n_body.c n_body_wrapper.c
