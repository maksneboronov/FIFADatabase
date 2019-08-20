USE fifaDataBase;

IF OBJECT_ID('dbo.Matches', 'U') IS NOT NULL  
	DROP TABLE dbo.Matches; 
	
CREATE TABLE dbo.Matches 
(  
	mid					INT				NOT NULL IDENTITY(0, 1),  
	tid1				INT				NOT NULL,  	
	tid2				INT				NOT NULL,  
	stid				INT				NULL,
	mdate				DATE			NOT NULL DEFAULT('12-12-2100'),  
	mg1					INT				NOT NULL DEFAULT(0),
	mg2					INT				NOT NULL DEFAULT(0),
	   	
	CONSTRAINT matchesPrimaryKeyMid PRIMARY KEY(mid),
	CONSTRAINT matchesForeignKeySTid FOREIGN KEY(stid) REFERENCES dbo.Stadium(stid) ON DELETE SET NULL,
	--CONSTRAINT matchesForeignKeyTid1 FOREIGN KEY(tid1) REFERENCES dbo.Teams(tid),
	--CONSTRAINT matchesForeignKeyTid2 FOREIGN KEY(tid2) REFERENCES dbo.Teams(tid),
);

