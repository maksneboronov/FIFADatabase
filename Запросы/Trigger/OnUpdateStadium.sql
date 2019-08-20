use fifaDataBase;

GO
CREATE TRIGGER OnUpdateStadium
ON StadiumsView
INSTEAD OF UPDATE
AS
BEGIN
	DECLARE @pid			INT;
	DECLARE @seatcount		INT;
	DECLARE @sectorcount	INT;
	DECLARE @name			NVARCHAR(30);
	DECLARE @town			NVARCHAR(30);	
	DECLARE @stid			INT;

	SELECT @stid = id, @pid = pid, @seatcount = seatCount, @sectorcount = sectorCount, @name = stName, @town = town FROM inserted;

	IF (@pid = -1)
		BEGIN
		UPDATE dbo.Stadium 
			SET sname = @name, sseatcount = @seatcount, stown = @town, tsectorcount = @sectorcount, tscheme = (SELECT scheme FROM inserted) 
			WHERE stid = @stid;			
		END;
	ELSE
		BEGIN
		UPDATE dbo.Stadium 
			SET pid = @pid, sname = @name, sseatcount = @seatcount, stown = @town, tsectorcount = @sectorcount, tscheme = (SELECT scheme FROM inserted) 
			WHERE stid = @stid;
		END;

	SELECT @stid = stid FROM dbo.Stadium WHERE sname LIKE @name AND @town LIKE stown;
END;