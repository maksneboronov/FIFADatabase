use fifaDataBase;

GO
CREATE PROCEDURE IsUsedStadium(
	@date	DATE,
	@stid	INT,
	@result INT output )
AS
BEGIN
	SELECT @result = COUNT(*) FROM dbo.Matches WHERE mdate = @date AND Stid = @stid;
END
