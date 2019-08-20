use fifaDataBase;

GO
CREATE PROCEDURE GetStadiumScheme
	@stid	INT,
	@scheme IMAGE OUTPUT
AS
BEGIN
	SELECT @scheme = tscheme FROM dbo.Stadium WHERE stid = @stid;
END
