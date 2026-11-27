--Assigntment 2 Task 2

\set ECHO all

DROP FUNCTION IF EXISTS get_region_supplier_balance(TEXT);

CREATE OR REPLACE FUNCTION get_region_supplier_balance(p_region TEXT)
RETURNS TEXT
LANGUAGE plpgsql
AS $$
DECLARE
    v_regionkey INTEGER;
    rec_low RECORD;
    rec_high RECORD;
    result TEXT;
BEGIN

    SELECT r_regionkey
    INTO v_regionkey
    FROM region
    WHERE UPPER(r_name) = UPPER(p_region)
    LIMIT 1;

    IF NOT FOUND THEN
        RETURN FORMAT('Region "%s" does not exist.', p_region);
    END IF;

    WITH sup AS(
        SELECT s.s_suppkey, BTRIM(s.s_name) AS s_name, s.s_acctbal
        FROM supplier s
        JOIN nation n ON n.n_nationkey = s.s_nationkey
        WHERE n.n_regionkey = v_regionkey
    )
    SELECT s_suppkey, s_name, s_acctbal
    INTO rec_low
    FROM sup
    ORDER BY s_acctbal ASC, s_suppkey
    LIMIT 1;

    IF rec_low IS NULL THEN
        RETURN FORMAT('No suppliers found in region "%s".',p_region);
    END IF;

    WITH sup AS (
        SELECT s.s_suppkey, BTRIM(s.s_name) AS s_name, s.s_acctbal
        FROM supplier s
        JOIN nation n ON n.n_nationkey = s.s_nationkey
        WHERE n.n_regionkey = v_regionkey
    )
    SELECT s_suppkey, s_name, s_acctbal
    INTO rec_high
    FROM sup
    ORDER BY s_acctbal DESC, s_suppkey
    LIMIT 1;

    result := FORMAT(
        'Supplier with lowest balance: %s, %s, %s.%sSupplier with highest balance: %s, %s, %s.',
        rec_low.s_suppkey,
        rec_low.s_name,
        TO_CHAR(rec_low.s_acctbal, 'FM$9G999G999G990D00'),
        E'\n',
        rec_high.s_suppkey,
        rec_high.s_name,
        TO_CHAR(rec_high.s_acctbal, 'FM$9G999G999G990D00')
    );

    RETURN result;
END;
$$;
\echo '--Tests--'
SELECT get_region_supplier_balance('ASIA');
SELECT get_region_supplier_balance('EUROPE');
SELECT get_region_supplier_balance('AMERICA');
SELECT get_region_supplier_balance('asia');          
SELECT get_region_supplier_balance('NO_SUCH_REGION');

\set ECHO none