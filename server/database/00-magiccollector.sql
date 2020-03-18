-- WARNING
DROP DATABASE magcol;
DROP OWNED BY dbamagcol;
DROP USER dbamagcol;
-- WARNING

create user dbamagcol with password 'magcol';
create database magcol owner dbamagcol;
