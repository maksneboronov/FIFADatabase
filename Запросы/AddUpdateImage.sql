use fifaDataBase;

--INSERT INTO dbo.Stadium(sname, stown, pid, tscheme)
--       SELECT
--             '1', '12', 0, IMG.BulkColumn AS Bytes
--       FROM
--             OPENROWSET(BULK N'F:\menu.ico', SINGLE_BLOB) AS IMG
UPDATE dbo.Stadium SET tscheme = 
		(SELECT IMG.BulkColumn AS Bytes
		FROM OPENROWSET(BULK N'F:\OPEN.png', SINGLE_BLOB) AS IMG)
	WHERE stid = 1;

Go
SELECT * FROM dbo.Stadium;