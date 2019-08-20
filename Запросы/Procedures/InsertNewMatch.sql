use fifaDataBase;

Go
CREATE PROCEDURE InsertNewMatch(
	@tid1		INT,
	@tid2		INT,
	@stid		INT,
	@mg1		INT,
	@mg2		INT,
	@date		DATE,
	@mid		INT	output		)
AS
BEGIN
	INSERT INTO dbo.Matches(tid1, tid2, Stid, mg1, mg2, mdate) VALUES(@tid1, @tid2, @stid, @mg1, @mg2, @date);
	SELECT @mid = mid FROM dbo.Matches WHERE tid1 = @tid1 AND @tid2 = tid2 AND @date = mdate;
END
	