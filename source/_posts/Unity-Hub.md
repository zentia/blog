---
title: UnityHub破解
date: 2019-03-03 04:49:42
tags:
  - Unity
---
退出UnityHub

	npm install -g asar
	cd C:\Program Files\Unity Hub\resources
	asar extract .\app.aser app
	rm -rf app.asar
	cd app/src/services/licenseService
	vim licenseClient.js

```
getLicenseInfo(callback){
	// load license
	// get latest data from licenseCore
	//licenseInfo.activated = licenseCore.getLicenseToken().length > 0; // 注释这行
	licenseInfo.activated = true; // 新增这行
	licenseInfo.flow = licenseCore.getLicenseKind();
}
```

	vim licenseCore.js

```
verifyLicenseData(xml){
	return new Promise((resolve, reject)=>
		resolve(true); //新增这行
		if (xml === ''){
		
		}
	)
}
```