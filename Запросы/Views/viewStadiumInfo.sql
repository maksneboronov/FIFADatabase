use fifaDataBase;

GO
IF OBJECT_ID('dbo.StadiumInfo', 'U') IS NOT NULL  
	DROP FUNCTION dbo.StadiumInfo; 

GO
CREATE VIEW dbo.StadiumInfo
AS
SELECT s.sname, s.pid, s.sseatcount, s.stown, s.tsectorcount, s.tscheme
FROM dbo.Stadium as s;

