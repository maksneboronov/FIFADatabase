USE fifaDataBase;

GO
CREATE TRIGGER OnInsertMatch
ON dbo.Matches
AFTER INSERT
AS
BEGIN
	DECLARE @date		DATE
	DECLARE @currDate	DATE

	SELECT @date = mdate FROM inserted;
	SELECT @currDate = CONVERT (date, GETDATE());

	IF (@date <= @currDate)
	BEGIN
		DECLARE @htid		INT
		DECLARE @atid		INT
		DECLARE @hgoals		INT
		DECLARE @agoals		INT
		DECLARE @hpoints	INT
		DECLARE @apoints	INT
		DECLARE @awon		INT
		DECLARE @hwon		INT
		DECLARE @adrawn		INT
		DECLARE @hdrawn		INT
		DECLARE @alose		INT
		DECLARE @hlose		INT
		DECLARE @agf		INT
		DECLARE @aga		INT
		DECLARE @hgf		INT
		DECLARE @hga		INT
		DECLARE @agames		INT
		DECLARE @hgames		INT
		DECLARE @result		INT

		SELECT @htid = tid1, @hgoals = mg1 FROM inserted;
		SELECT @atid = tid2, @agoals = mg2 FROM inserted;

		SELECT @apoints = tpoints, @agames = tgame, @alose = tlost, @adrawn = tdrawn, @awon = twon, @aga = tga, @agf = tgf FROM dbo.Teams WHERE tid = @atid;
		SELECT @hpoints = tpoints, @hgames = tgame, @hlose = tlost, @hdrawn = tdrawn, @hwon = twon, @hga = tga, @hgf = tgf FROM dbo.Teams WHERE tid = @htid;

		SET @result = @hgoals - @agoals;

		IF (@result < 0)
			BEGIN
			SET @awon = @awon + 1;
			SET @hlose = @hlose + 1;
			SET @apoints = @apoints + 3;
			END
		ELSE IF (@result > 0)
			BEGIN
			SET @hwon = @hwon + 1;
			SET @alose = @alose + 1;
			SET @hpoints = @hpoints + 3;
			END
		ELSE
			BEGIN
			SET @adrawn = @adrawn +1;			
			SET @hdrawn = @hdrawn + 1;
			SET @apoints = @apoints + 1;
			SET @hpoints = @hpoints + 1;
			END

		UPDATE dbo.Teams SET tgame = @hgames + 1, tga = @hga + @hgoals, tgf = @hgf + @agoals, tpoints = @hpoints, twon = @hwon, tdrawn = @hdrawn, tlost = @hlose WHERE tid = @htid;
		UPDATE dbo.Teams SET tgame = @agames + 1, tga = @aga + @agoals, tgf = @hgf + @hgoals, tpoints = @apoints, twon = @awon, tdrawn = @adrawn, tlost = @alose WHERE tid = @atid;
	END;
END;