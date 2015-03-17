# Introduction #

Welcome to the vsftpdSQL project! This is currently in pre alpha stage, but I hope the project will rapidly evolve into a stable ftp server.

# Details #

The vsftpdSQL ftp server is very similar to the vsftp ftp server. The main difference is that all administration tasks will be performed via altering the database and all events are written to the database.

Features:
  * administration via database
  * events logging to database
  * MySQL support
  * PostgreSQL support
  * almost easy and intuitive configuration (well, you need a DB, a user and the proper schema :)
