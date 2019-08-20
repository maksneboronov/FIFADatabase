use fifaDataBase


GO
CREATE VIEW TeamsView
AS 
SELECT tid as Id, tname as TeamName, tcoach as Coach, tgroup as TeamGroup, tgame as Games, twon as Wons, tdrawn as Drawns, tlost as Loses, tgf as Gf, tga as Ga, tpoints as Points
FROM dbo.Teams;
