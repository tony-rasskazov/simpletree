/*select * from vehicle_models;
*/
insert into vehicle_models (id, model, v_id) values (2, '3wergwfglsdfg', 0);
CREATE TABLE public.vehicle_species
(
    id serial NOT NULL,
    model text,
    v_id bigint NOT NULL,
    PRIMARY KEY (id)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE public.vehicle_species
    OWNER to postgres;