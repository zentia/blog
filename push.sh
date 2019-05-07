#!/bin/sh

hexo d g
sleep 2
cd public
git add .
sleep 2
git commit -m "auto"
sleep 2
git push