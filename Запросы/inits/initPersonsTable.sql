USE fifaDataBase;

IF OBJECT_ID('dbo.Persons', 'U') IS NOT NULL  
	DROP TABLE dbo.Persons; 
	
CREATE TABLE dbo.Persons 
(  
	pid					INT				NOT NULL IDENTITY(0, 1),  
	pname				NVARCHAR(50)	NOT NULL,  	
	ppassport			VARCHAR(10)		NOT NULL,  
	pphone				VARCHAR(11)		NULL,  
	ppriority			CHAR			NOT NULL DEFAULT('A'),
	   	
	CONSTRAINT personsPrimaryKeyPid PRIMARY KEY(pid),
	CONSTRAINT personsUniquePpassport UNIQUE(ppassport)	
);

INSERT INTO dbo.Persons(pname, ppassport, pphone, ppriority) VALUES ('Неборонов Максим Максимович', '9999999999', '79031999621', 'A');
