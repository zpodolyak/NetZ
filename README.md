epoll_socket_server
===================

A simple, Linux, non-blocking socket server that implements the Reactor pattern. 

Background
==========

This project was originally started as part of a turn-based mobile game similar to Words With Friends. I started researching the various technical challenges involved in creating a mobile game server and I came across hot topics like Node.js, Nginx, NoSQL, WebSocket, game matching, push notifications, just to name a few. Then I looked around for hosting packages that handle these things for you, like Amazon AWS, and I was amazed how much these services can end up costing. I started thinking, how much of an effort would it be to create these services from scratch? Or at least, are there any alternatives to coding your game server in Javascript? I read up on things like libevent and the C10K problem. Then about weird things like epoll and kqueue. And so epoll_socket_server was born.

DISCLAIMER
==========
It has not yet been fully tested, especially in a production, heavy-load environment. It's more of a proof of concept,
or rather an exercise more than anything. I intend to add more features to it, but luckily you can too.
