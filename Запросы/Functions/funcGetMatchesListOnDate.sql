use fifaDataBase;

--GO
--IF OBJECT_ID('dbo.GetMatchesListOnDate', 'U') IS NOT NULL  
--	DROP FUNCTION dbo.GetMatchesListOnDate; 

--GO
--CREATE FUNCTION GetMatchesListOnDate(@datetime date)
--RETURNS TABLE
--AS
-- RETURN 
-- (	
--	SELECT m.mdate
--	FROM dbo.Matches as m
--	WHERE DAY(m.mdate) = DAY(@datetime)
-- )

 SELECT *
 FROM GetMatchesListOnDate('1-1-2017') as d
 ORDER BY d.mdate; 