use fifaDataBase;

--GO
--IF OBJECT_ID('dbo.GetStadiumInfo', 'U') IS NOT NULL  
--	DROP FUNCTION dbo.GetStadiumInfo; 

--GO
--CREATE FUNCTION dbo.GetStadiumInfo(@name nvarchar(30), @town nvarchar(30))
--RETURNS TABLE
--AS
--RETURN
--(
--	SELECT s.sname, s.stown, s.sseatcount, s.tsectorcount, s.tscheme
--	FROM dbo.StadiumInfo as s
--	WHERE s.stown LIKE @town AND s.sname LIKE @name
--)

GO
SELECT * FROM dbo.GetStadiumInfo('Открытие Арена', 'Москва');