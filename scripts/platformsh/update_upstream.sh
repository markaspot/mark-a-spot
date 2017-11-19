#!/usr/bin/env bash

git fetch --all
git merge -X theirs --squash dc/8.x
git reset README.md
git checkout -- README.md
php scripts/platformsh/Platformify.php
git add .
