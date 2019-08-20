use fifaDataBase;

GO
IF OBJECT_ID('dbo.TeamsInfo', 'U') IS NOT NULL  
	DROP VIEW dbo.TeamsInfo; 

GO
CREATE VIEW dbo.TeamsInfo
AS
SELECT t.tcoach, t.tname, t.tgroup, t.tgame, t.tpoints, t.twon, t.tdrawn, t.tlost, t.tga, t.tgd
FROM dbo.Teams as t;

