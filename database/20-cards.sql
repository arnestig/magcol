\c magcol;

create sequence sq_cards_id start with 1;
alter sequence sq_cards_id owner to dbamagcol;

create table cards (
        card_id integer primary key default nextval('sq_cards_id'),
        card_expansion varchar,
        card_name varchar,
        card_rarity varchar,
        card_type varchar,
        card_subtype varchar,
        card_oracle varchar,
        card_cost varchar,
        card_image_ref varchar
        );
alter table cards owner to dbamagcol;

-- 'Cards' stored procedures
--
-- add_card
create or replace function add_card(
    c_expansion varchar,
    c_name varchar,
    c_rarity varchar,
    c_type varchar,
    c_subtype varchar,
    c_oracle varchar,
    c_cost varchar,
    c_image_ref varchar )
returns void as $$
declare
begin
    insert into cards( 
        card_expansion, 
        card_name, 
        card_rarity, 
        card_type, 
        card_subtype, 
        card_oracle, 
        card_cost, 
        card_image_ref ) 
    values( 
        c_expansion, 
        c_name, 
        c_rarity, 
        c_type, 
        c_subtype, 
        c_oracle, 
        c_cost, 
        c_image_ref );
    
end;
$$ language plpgsql;
alter function add_card(varchar,varchar,varchar,varchar,varchar,varchar,varchar,varchar) owner to dbamagcol;

-- get_cards
create or replace function get_cards(
    c_search_string varchar )
returns SETOF refcursor as $$
declare
ref1 refcursor;
begin
open ref1 for
    SELECT * FROM cards
    WHERE 
        (c_search_string is NULL or (lower(cards.card_name) ~ ('^' || lower(c_search_string))))
    ORDER BY cards.card_name LIMIT 100;
return next ref1;
end;
$$ language plpgsql;
alter function add_card(varchar) owner to dbamagcol;

