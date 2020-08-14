# Change Distiller
For the Change Distiller we implemented a docker container which includes the Change Distiller
and a Spring Boot -based API-system.

In this submission the Release-folder includes the src-folder which has the code we developed and the results-folder,
which includes sample-analyses from our tool. The Change-distiller -tool is also in the Release-folder, in the 
tools-change-distiller-folder, which is indicated by it having the Dockerfile in it.

The requirements for this to work is to have Docker installed and to have the contents of the tools-change-distiller-folder 
which can be found in our project repository. It should work with most of the recent versions but we make no guarantees. 
The Docker-version it was tested on is 18.09.0.

Disclaimer. In this document we are going to use the Ognl-Commons-project as the example for the functionality of our project, 
but the information shown may not represent the actual results gotten from analyzing the project.

The API has three functions: analyze, check and getresults.

## ANALYZE
The analyze functionality is used to call Change Distiller with the parameters included in the
url. The parameters are repo, hash1, hash2 and seq. All parameters are mandatory. Repo is the address of the git-repository 
(e.g. https://github.com/apache/commons-ognl.git). Hash1 is the Sha of the commit you want to start the analysis (e.g. d819159cc41f01ae26fb0f0671338c1ae4b2c1337). 
Hash2 is the sha of the commit until which you want to analyze the differences (e.g. 99dcc4b301c3c59a5616263a9d9b055e3ebc3928). 
Note that for the analyze to be succesfull hash1 must point to a newer commit than hash2. Seq (short for sequential) can 
either be true of false. It decides if the commits are analyzed sequentially or not. Non-sequential analysis looks at differences between 
the two commits. Sequential on the other hand looks through all the commit-pairs and analyzes their differences. 

Example of sequentiality:

	You have commits 1, 2, 3, 4, and you are analyzing differences between 1 and 4.

	Non-sequential analysis (seq=false) results in a list of differences between 1 and 4.

	Sequential analysis (seq=true) results in a list of difference-lists. The list would include differences between 1->2, 2->3 and 3->4.

If the analysis is succesful the container responds with a 200 OK HTTP-Response and a simple text saying Analyze succesfull.


## CHECK
The check functionality is used to poll whether the results of the analysis are ready. The check always results in a 200 OK HTTP-Response, 
which includes a simple json to check if the analysis is ready.

The json is in the following format:
	
	'''
	{ 
		"content": true, 
		"repo": "Changes ready"
	}
	'''

It has two key-value pairs: repo and content. Repo says if the changes are true in a string and content is a boolean which is 
true if the analysis is ready and false if it is not.

## GETRESULTS
The getresults functionality is used to access the results found in the analysis. The getresults-call
return a json-object with the information the Change Distiller has gathered. 

The json is in the following form (non-sequential):

	'''
	{
		"leftCommit": {
			"Hash": "d819159cc41f01ae26fb0f0671338c1ae4b2c133",
			"Date": "18-08-2018 16:08:16"
		},
		"rightCommit": {
			"Hash": "99dcc4b301c3c59a5616263a9d9b055e3ebc3928",
			"Date": "07-52-2017 17:52:34"
		},
		"changedFiles": {
			"Changed": [
			{
				"path": "src/test/java/org/apache/commons/ognl/internal/MethodPermCacheTest.java",
				"fileName": "MethodPermCacheTest.java",
				"changeList": [
					[
					"Update: org.apache.commons.ognl.internal.MethodPermCacheTest.testGetPublicMethod_returnsTrue()",
					"HIGH",
					"METHOD_RENAMING"
					],
					[
					"Delete: Cache<Method, Boolean> cache = createCache(allowAllSecurityManager);",
					"MEDIUM",
					"STATEMENT_DELETE"
					]
				]
			}
			]
		},
		"notChangedFiles": {
			"Not Changed": [
				{
					"path": "LICENSE.txt",
					"fileName": "LICENSE.txt"
				},
				{
					"path": "/src/NOTICE.txt",
					"fileName": "NOTICE.txt"
				}
			]
		}
	}
	'''

The json has the name of hashes and times of the commits and a list of the changed files and files that have not been changes. If the analysis is sequential, the file would have a json-list of objects like this.

### USAGE EXAMPLES
Here is a simple way to test out the functionalities:
First, initiate the docker container (In the folder where the Dockerfile is).
Name of the image is of course optional.

	docker build -f Dockerfile -t cd1 .
	docker run -p 8080:8080 cd1

Then open localhost:8080 (The IP-address may vary if using a virtual box).
The functionalities can be tested for example with the following urls:
	
	http://localhost:8080/analyze?repo=https://github.com/apache/commons-ognl.git&hash1=d819159cc41f01ae26fb0f0671338c1ae4b2c133&hash2=99dcc4b301c3c59a5616263a9d9b055e3ebc3928&seq=true
	http://localhost:8080/check
	http://localhost:8080/getresults

	http://localhost:8080/analyze?repo=https://github.com/apache/commons-ognl.git&hash1=d819159cc41f01ae26fb0f0671338c1ae4b2c133&hash2=99dcc4b301c3c59a5616263a9d9b055e3ebc3928&seq=false
	http://localhost:8080/check
	http://localhost:8080/getresults


