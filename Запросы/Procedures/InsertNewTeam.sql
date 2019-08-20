use fifaDataBase;

	
Go
CREATE PROCEDURE InsertNewTeam(
	@teamName		NVARCHAR(30),
	@coach			NVARCHAR(30),
	@group			CHAR(1),
	@game			INT,
	@won			INT,
	@drawn			INT,
	@lose			INT,
	@gf				INT,
	@ga				INT,
	@points			INT,
	@tid			INT	output		)
AS
BEGIN
	INSERT INTO TeamsView(TeamName, Coach, TeamGroup, Games, Wons, Drawns, Loses, Gf, Ga, Points) 
		VALUES (@teamName, @coach, @group, @game, @won, @drawn, @lose, @gf, @ga, @points);
	SELECT @tid = tid FROM dbo.Teams WHERE tname LIKE @teamName AND tcoach LIKE @coach;
END