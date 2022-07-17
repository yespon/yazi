<?php

$host = '127.0.0.1';
$port = 8080;

$sockfd = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
socket_connect($sockfd, $host, $port);

send_data($sockfd, "hello c++");
$resp = socket_read($sockfd, 1024);
echo $resp . "\n";

send_data($sockfd, "this is me");
$resp = socket_read($sockfd, 1024);
echo $resp . "\n";

send_data($sockfd, "there you are!");
$resp = socket_read($sockfd, 1024);
echo $resp . "\n";

socket_close($sockfd);

function send_data($sockfd, $data)
{
    $pack = pack('a8', 'work');
    $cmd = 1;
    $len = strlen($data);
    $pack .= pack('i2', $cmd, $len);
    $pack .= $data;
    socket_write($sockfd, $pack);
}