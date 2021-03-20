#!/bin/bash

USER="$1"

scp $USER@134.209.145.155:/etc/nginx/sites-available/azkaban.in conf/azkaban.in 
scp pi@bulbasaur.local:/etc/nginx/sites-available/alohomora.azkaban.in conf/alohomora.azkaban.in


