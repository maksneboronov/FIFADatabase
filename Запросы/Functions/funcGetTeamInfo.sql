use fifaDataBase;

GO
IF OBJECT_ID('dbo.GetTeamInfo', 'U') IS NOT NULL  
	DROP FUNCTION dbo.GetTeamInfo; 

GO
CREATE FUNCTION dbo.GetTeamInfo(@name nvarchar(30))
RETURNS TABLE
AS
RETURN
(
	SELECT t.tcoach, t.tgroup, t.tgame, t.tpoints, t.twon, t.tdrawn, t.tlost, t.tga, t.tgd
	FROM dbo.TeamsInfo as t
	WHERE t.tname LIKE @name
)

GO
SELECT * FROM dbo.GetTeamInfo('Россия');