use fifaDataBase;

--GO
--IF OBJECT_ID('dbo.GetGroup', 'U') IS NOT NULL  
--	DROP FUNCTION dbo.GetGroup; 

--GO
--CREATE FUNCTION GetGroup(@groupname char)
--RETURNS TABLE
--AS
-- RETURN 
-- (
--	SELECT res.tname, res.tcoach, res.tpoints, res.tgame, res.twon, res.tdrawn, res.tlost, res.tga, res.tgd, res.tgf
--	FROM dbo.Teams as res
--	WHERE res.tgroup = @groupname	
-- )

 SELECT *
 FROM GetGroup('C') as res
 ORDER BY res.tpoints, res.twon, res.tdrawn, res.tlost, res.tga, res.tgd, res.tgf;