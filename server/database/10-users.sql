\c magcol;

create sequence sq_user_id start with 1;
alter sequence sq_user_id owner to dbamagcol;

create table users (
        user_id integer primary key default nextval('sq_user_id'),
        user_name varchar,
        user_email varchar
        );
alter table users owner to dbamagcol;

insert into users (user_name,user_email) values( 'admin', '' );
