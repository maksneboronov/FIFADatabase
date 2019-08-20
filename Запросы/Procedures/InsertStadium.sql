use fifaDataBase;

GO

CREATE PROCEDURE InsertStadium	(
	@pid			INT,
	@seatcount		INT,
	@sectorcount	INT,
	@name			NVARCHAR(30),
	@town			NVARCHAR(30),
	@map			IMAGE,
	@stid			INT		output	)
AS
BEGIN
	IF (@pid = -1)
		BEGIN
		INSERT INTO dbo.Stadium(sname, sseatcount, stown, tsectorcount, tscheme) 
			VALUES(@name, @seatcount, @town, @sectorcount, @map);
		END;
	ELSE
		BEGIN
		INSERT INTO dbo.Stadium(sname, sseatcount, stown, tsectorcount, tscheme, pid) 
			VALUES(@name, @seatcount, @town, @sectorcount, @map, @pid);
		END;

	SELECT @stid = stid FROM dbo.Stadium WHERE sname LIKE @name AND @town LIKE stown;
END;