\c magcol;

create sequence sq_price_id start with 1;
alter sequence sq_price_id owner to dbamagcol;

create table prices (
        price_id integer primary key default nextval('sq_price_id'),
        card_id uuid not null references cards(card_id),
        price_date date not null default CURRENT_DATE,
        price_usd real default 0.0,
        price_usd_foil real default 0.0
        );
alter table prices owner to dbamagcol;

-- 'price' stored procedures
--
-- add_price
create or replace function add_price(
    c_id uuid,
    p_usd real,
    p_usd_foil real )
returns void as $$
declare
begin
    insert into prices(
        card_id,
        price_usd,
        price_usd_foil)
    values(
        c_id,
        p_usd,
        p_usd_foil);

end;
$$ language plpgsql;
alter function add_price(uuid,real,real) owner to dbamagcol;

-- get_prices
create or replace function get_prices_by_card_id(
    c_id uuid,
    p_from_date date,
    p_to_date date)
returns SETOF refcursor as $$
declare
ref1 refcursor;
begin
open ref1 for
    SELECT card_id, price_date, price_usd, price_usd_foil FROM prices
    WHERE
        price_date >= p_from_date
        AND price_date <= p_to_date
        AND c_id == card_id
    ORDER BY price_date;
return next ref1;
end;
$$ language plpgsql;

alter function get_prices_by_card_id(uuid,date,date) owner to dbamagcol;

