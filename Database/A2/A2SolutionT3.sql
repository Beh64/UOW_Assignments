
--Assignment 2 Task 3--

\set ECHO all

DROP TRIGGER IF EXISTS trg_fix_acctbal ON customer;
DROP FUNCTION IF EXISTS fix_acctbal();

CREATE OR REPLACE FUNCTION fix_acctbal()
RETURNS TRIGGER
LANGUAGE plpgsql
AS $$
BEGIN
    IF NEW.c_acctbal < 0 THEN
        NEW.c_acctbal := 0;
    END IF;
    RETURN NEW;
END;
$$;

CREATE TRIGGER trg_fix_acctbal
BEFORE INSERT OR UPDATE OF c_acctbal ON customer
FOR EACH ROW
EXECUTE FUNCTION fix_acctbal();

DELETE FROM customer WHERE c_custkey IN (45001, 45002);

\echo ---TEST 1: INSERT negative (expect 0)---
INSERT INTO customer
(c_custkey, c_name, c_address, c_nationkey, c_phone, c_acctbal, c_mktsegment, c_comment)
VALUES
(45001, 'Customer#000045001', 'Addr 1', 1, '28-123-4567', -50, 'AUTOMOBILE', 'Negative insert');
SELECT c_custkey, c_acctbal FROM customer WHERE c_custkey = 45001;

\echo ---TEST 2: INSERT non-negative (expect 200.25 unchanged)---
INSERT INTO customer
(c_custkey, c_name, c_address, c_nationkey, c_phone, c_acctbal, c_mktsegment, c_comment)
VALUES
(45002, 'Customer#000045002', 'Addr 2', 1, '28-987-6543', 200.25, 'AUTOMOBILE', 'Positive insert');
SELECT c_custkey, c_acctbal FROM customer WHERE c_custkey = 45002;


\echo ---TEST 3: UPDATE to negative (expect 0)---
UPDATE customer SET c_acctbal = -100 WHERE c_custkey = 45001;
SELECT c_custkey, c_acctbal FROM customer WHERE c_custkey = 45001;


\echo ---TEST 4: UPDATE to non-negative (expect 123.45 unchanged)---
UPDATE customer SET c_acctbal = 123.45 WHERE c_custkey = 45001;
SELECT c_custkey, c_acctbal FROM customer WHERE c_custkey = 45001;


\echo ---TEST 5: UPDATE other column (trigger should NOT fire)---
UPDATE customer SET c_comment = 'Other column update' WHERE c_custkey = 45001;
SELECT c_custkey, c_acctbal, c_comment FROM customer WHERE c_custkey = 45001;

-- End
\set ECHO none