#!/bin/bash

ps -ef|grep main|grep -v grep|grep -v vscode|awk -F ' ' '{print $2}'|xargs kill -9

rm -rf log/*.log

