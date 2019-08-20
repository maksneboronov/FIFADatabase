USE fifaDataBase;

IF OBJECT_ID('dbo.Auth', 'U') IS NOT NULL  
	DROP TABLE dbo.Auth; 
	
CREATE TABLE dbo.Auth 
(  
	aid					INT				NOT NULL IDENTITY(0, 1),  
	pid					INT				NOT NULL,  	
	alogin				NVARCHAR(30)	NOT NULL,  
	apassword			NVARCHAR(30)	NOT NULL,  
	   	
	CONSTRAINT authPrimaryKeyAid PRIMARY KEY(aid),
	CONSTRAINT authUniqueLogin UNIQUE(alogin),
	CONSTRAINT authForeignKeyPid FOREIGN KEY(pid) REFERENCES dbo.Persons(pid) ON DELETE CASCADE ON UPDATE CASCADE,
);


INSERT INTO dbo.Auth(pid, alogin, apassword) VALUES (0, 'root', 'admin');
