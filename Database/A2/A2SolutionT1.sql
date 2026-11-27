--ASSIGNMENT--

\set ECHO all

DROP PROCEDURE IF EXISTS customer_order_value(TEXT);    --Removes the procedure if it already exist
DROP TABLE IF EXISTS tmp_customer_order_value;

CREATE TEMP TABLE tmp_customer_order_value      --Creates a temporary table to hold the final results
(                           --All the columns are TEXT
    "Customer name" TEXT,
    "Account balance" TEXT,
    "Total order value" TEXT,
    "Comment" TEXT
) ON COMMIT PRESERVE ROWS;  --temp table lives for the entire session unless dropped

CREATE OR REPLACE PROCEDURE customer_order_value(nation_name TEXT) --drops the procedure if exists then creates it
LANGUAGE plpgsql    --coding language 
AS $$       
DECLARE
    v_nationkey INTEGER;    --create variable v_nationkey and v_avg
    v_avg NUMERIC;
BEGIN       --select the n_nationkey and copy it to v_nationkey.
    SELECT n_nationkey
    INTO v_nationkey
    FROM nation
    WHERE UPPER(n_name) = UPPER(nation_name)    --link the nation_name input to the n_name in nation
    LIMIT 1;    --takes only 1 row

    IF NOT FOUND THEN   --if the input isnt found, then raise notice
        RAISE NOTICE 'Nation name: "%" does not exist.', nation_name;
        RETURN;
    END IF;

    CREATE TEMP TABLE cust_total    --creates a temporary table called cust_total
    ON COMMIT DROP AS           --drop table automatically when the code finishes running
    SELECT
        o.o_custkey AS custkey,
        SUM(o.o_totalprice)::NUMERIC AS total_order_value   --convert to numeric as need decimals
    FROM orders o   --makes o the alias for orders. makes c alias for customer in join
    JOIN customer c ON c.c_custkey = o.o_custkey    --join specific tables which are the custkey from c and o
    WHERE c.c_nationkey = v_nationkey   --condition where the customer nationkey is the same one as the input
    GROUP BY o.o_custkey;       --groups the values with the identifier o.o_custkey

    --Calculate national average across those per customer totals.
    SELECT AVG(total_order_value)
    INTO v_avg
    FROM cust_total;

    --If no customers with orders exists then raise notice
    IF v_avg IS NULL THEN
        RAISE NOTICE 'Nation name: "%" has no customers with orders.', nation_name;
        RETURN;
    END IF;

    --Use their total order value and compare national average
    --Display according to specifications

    DELETE FROM tmp_customer_order_value;   --remove any data in the table

    INSERT INTO tmp_customer_order_value    --insert according to column
    (   --by using "" it has to be case sensitive
        "Customer name",
        "Account balance",
        "Total order value",
        "Comment"
    )
    SELECT --select information in the same order as insert above
        c.c_name,    --FM removes spaces and padding , G is comma, D is decimal point
        TO_CHAR(c.c_acctbal::NUMERIC, 'FM$9G999G999G990D00') AS acctbal_fmt,
        TO_CHAR(ct.total_order_value, 'FM$9G999G999G990D00') AS total_fmt,
        CASE   --case gives multiple conditions like if else -> when then
            WHEN ct.total_order_value > v_avg
                THEN 'Above nation average of ' || TO_CHAR(v_avg, 'FM$9G999G999G990D00')
            WHEN ct.total_order_value < v_avg 
                THEN 'Below nation average of ' || TO_CHAR(v_avg, 'FM$9G999G999G990D00')
            ELSE 'Equal to nation average of ' || TO_CHAR(v_avg, 'FM$9G999G999G990D00')
        END
    FROM cust_total ct
    JOIN customer c ON c.c_custkey = ct.custkey
    ORDER BY c.c_name;
END;
$$;

\set ECHO none
