\c magcol;

create sequence sq_collection_id start with 1;
alter sequence sq_collection_id owner to dbamagcol;

create table collection (
        collection_id integer primary key default nextval('sq_collection_id'),
        collection_usr_id smallint not null references users,
        collection_card integer not null references cards,
        collection_quality varchar,
        collection_quantity smallint
        );
alter table collection owner to dbamagcol;

-- 'Collection' stored procedures
--
-- add_card_to_collection
create or replace function add_card_to_collection(
    c_user smallint,
    c_card_name varchar,
    c_expansion varchar )
returns void as $$
declare
    c_card_id integer;
begin
    SELECT INTO c_card_id card_id FROM cards WHERE cards.card_name = c_card_name AND cards.card_expansion = c_expansion;
    insert into collection( 
        collection_usr_id, 
        collection_card, 
        collection_quality, 
        collection_quantity )
    values( 
        c_user,
        c_card_id,
        'NM',
        1 );
end;
$$ language plpgsql;
alter function add_card_to_collection(smallint,varchar,varchar) owner to dbamagcol;

-- remove_card_from_collection
create or replace function remove_card_from_collection(
    c_user smallint,
    c_card_id integer )
returns void as $$
begin
    DELETE FROM collection WHERE collection_id = c_card_id AND c_user = collection_usr_id;
end;
$$ language plpgsql;
alter function remove_card_from_collection(smallint,integer) owner to dbamagcol;

-- get_collection
create or replace function get_collection(
    c_user smallint,
    c_search_string varchar )
returns SETOF refcursor as $$
declare
    ref1 refcursor;
begin
open ref1 for
    SELECT * FROM collection LEFT OUTER JOIN cards ON (collection.collection_card = cards.card_id)
    WHERE 
        (c_search_string is NULL or (lower(cards.card_name) ~ lower(c_search_string)))
    ORDER BY cards.card_name;
return next ref1;
end;
$$ language plpgsql;
alter function get_collection(smallint,varchar) owner to dbamagcol;
