USE fifaDataBase;

GO
CREATE TRIGGER OnUpdateMatch
ON dbo.Matches
AFTER UPDATE
AS
BEGIN
	DECLARE @atid1	INT
	DECLARE @atid2	INT
	DECLARE @htid1	INT
	DECLARE @htid2	INT

	DECLARE @prevDate	DATE
	DECLARE @newDate	DATE
	DECLARE @currDate	DATE

	SELECT @prevDate = mdate, @atid1 = tid2, @htid1 = tid1 FROM deleted;
	SELECT @newDate = mdate, @atid2 = tid2, @htid2 = tid1 FROM inserted;

	SELECT @currDate = CONVERT (date, GETDATE());

	DECLARE @state INT
	SET @state = 0;

	IF (@newDate >= @currDate AND @prevDate < @currDate OR (@atid1 <> @atid2 OR @htid1 <> @htid2))
		SET @state = -1;
	ELSE IF (@newDate < @currDate AND @prevDate >= @currDate)
		SET @state = 1;

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

	IF (@state <> 0)
	BEGIN
		SELECT @htid = tid1, @hgoals = mg1 FROM deleted;
		SELECT @atid = tid2, @agoals = mg2 FROM deleted;

		SELECT @apoints = tpoints, @agames = tgame, @alose = tlost, @adrawn = tdrawn, @awon = twon, @aga = tga, @agf = tgf FROM dbo.Teams WHERE tid = @atid;
		SELECT @hpoints = tpoints, @hgames = tgame, @hlose = tlost, @hdrawn = tdrawn, @hwon = twon, @hga = tga, @hgf = tgf FROM dbo.Teams WHERE tid = @htid;

		SET @result = @hgoals - @agoals;

		IF (@result < 0)
			BEGIN
			SET @awon = @awon + @state;
			SET @hlose = @hlose + @state;
			SET @apoints = @apoints + 3 * @state;
			END
		ELSE IF (@result > 0)
			BEGIN
			SET @hwon = @hwon + @state;
			SET @alose = @alose + @state;
			SET @hpoints = @hpoints + 3 * @state;
			END
		ELSE
			BEGIN
			SET @adrawn = @adrawn + @state;			
			SET @hdrawn = @hdrawn + @state;
			SET @apoints = @apoints + @state;
			SET @hpoints = @hpoints + @state;
			END

		UPDATE dbo.Teams SET tgame = @hgames + @state, tga = @hga + @state * @hgoals, tgf = @hgf + @state *  @agoals, tpoints = @hpoints, twon = @hwon, tdrawn = @hdrawn, tlost = @hlose WHERE tid = @htid;
		UPDATE dbo.Teams SET tgame = @agames + @state, tga = @aga + @state * @agoals, tgf = @hgf + @state *  @hgoals, tpoints = @apoints, twon = @awon, tdrawn = @adrawn, tlost = @alose WHERE tid = @atid;
	END;

	IF (@newDate < @currDate AND @prevDate >= @currDate AND (@atid1 <> @atid2 OR @htid1 <> @htid2))
	BEGIN
		SET @state = 1;
		SELECT @htid = tid1, @hgoals = mg1 FROM inserted;
		SELECT @atid = tid2, @agoals = mg2 FROM inserted;

		SELECT @apoints = tpoints, @agames = tgame, @alose = tlost, @adrawn = tdrawn, @awon = twon, @aga = tga, @agf = tgf FROM dbo.Teams WHERE tid = @atid;
		SELECT @hpoints = tpoints, @hgames = tgame, @hlose = tlost, @hdrawn = tdrawn, @hwon = twon, @hga = tga, @hgf = tgf FROM dbo.Teams WHERE tid = @htid;
		
		SET @result = @hgoals - @agoals;

		IF (@result < 0)
			BEGIN
			SET @awon = @awon + @state;
			SET @hlose = @hlose + @state;
			SET @apoints = @apoints + 3 * @state;
			END
		ELSE IF (@result > 0)
			BEGIN
			SET @hwon = @hwon + @state;
			SET @alose = @alose + @state;
			SET @hpoints = @hpoints + 3 * @state;
			END
		ELSE
			BEGIN
			SET @adrawn = @adrawn + @state;			
			SET @hdrawn = @hdrawn + @state;
			SET @apoints = @apoints + @state;
			SET @hpoints = @hpoints + @state;
			END

		UPDATE dbo.Teams SET tgame = @hgames + @state, tga = @hga + @state * @hgoals, tgf = @hgf + @state *  @agoals, tpoints = @hpoints, twon = @hwon, tdrawn = @hdrawn, tlost = @hlose WHERE tid = @htid;
		UPDATE dbo.Teams SET tgame = @agames + @state, tga = @aga + @state * @agoals, tgf = @hgf + @state *  @hgoals, tpoints = @apoints, twon = @awon, tdrawn = @adrawn, tlost = @alose WHERE tid = @atid;
	END;
END;