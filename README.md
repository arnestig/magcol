# magcol
Magic: The Gathering collection manager

# Prerequisites
For running and developing magcol you will need the following packages:
 Server:
 - php
 - postgresql
 - perl
 Client:
 - pHash
 - opencv

Installing on a Debian-based system you can:
```
apt-get install php7.3 perl libdbd-pg-perl postgresql postgresql-contrib
```

Update the file /etc/postgresql/11/main/pg_hba.conf and set trust as authentication method for local sessions

Restart the service via /etc/init.d/postgresql restart

Connecting to the database using the following command:
```
psql magcol dbamagcol
```

# Development
start a tiny php server for development:
```
php -S 127.0.0.1:8080 -t www/
```

run requests via curl for example:
```
curl http://127.0.0.1:8080
```

purge the database and re-populate:
```
for sql in $(ls server/database/*.sql); do sudo -u postgres psql -f $sql; done
```
