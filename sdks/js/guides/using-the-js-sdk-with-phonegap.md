# PhoneGap (Cordova) and StackMob

You can use the StackMob JS SDK with PhoneGap.

There are two things you'll need to do.

# 1 of 2. Include JS Dependencies

To use the StackMob JS SDK with PhoneGap, simply include the StackMob JS SDK and its dependencies along with the rest of your project.  Assuming you have saved local copies of the files below, add them to your PhoneGap app's `<head>` tags

```xml
<head>
	<!-- StackMob dependencies -->
	<script type="text/javascript" src="jquery.min.js"></script>
	<script type="text/javascript" src="json2-min.js"></script>
	<script type="text/javascript" src="underscore-1.3.3-min.js"></script>
	<script type="text/javascript" src="backbone-0.9.2-min.js"></script>
	<script type="text/javascript" src="2.5.3-crypto-sha1-hmac.js"></script>
	<script type="text/javascript" src="REPLACE THIS WITH THE LATEST STACKMOB JS FILE.js"></script>
</head>
```

Here are download links for the above files. 

# 2 of 2. Whitelist External Domains

Whitelist the StackMob API servers so that your PhoneGap app can make calls to your REST API.  Below are the settings for iOS and Android for convenience.

Visit <a href="http://docs.phonegap.com/en/1.8.1/guide_whitelist_index.md.html#Domain%20Whitelist%20Guide" target="_blank">PhoneGap's docs on how to allow External Hosts</a>

## iOS PhoneGap

Look for `Cordova.plist` or `PhoneGap.plist` in the `Supporting Files` folder.  Add the following:

```javascript
api.mob1.stackmob.com
*.stackmobapp.com
```  

## Android

Look for `cordova.xml` or `PhoneGap.xml` in the `/res/xml` folder.  Add the following:

```xml
<cordova>
	...
	<!--  
	access elements control the Android whitelist.  
	Domains are assumed blocked unless set otherwise
	 -->
  <access origin="stackmobapp.com" subdomains="true"/>
  <access origin="api.mob1.stackmob.com" subdomains="true"/>
  ....
</cordova>
```

# Making API Calls

After you've setup the above, use the JS SDK as you normally would!  So, get started by reading the <a href="https://developer.stackmob.com/tutorials/js" target="_blank">JS SDK Tutorial</a> and the <a href="https://developer.stackmob.com/stackmob-js-sdk/api-docs" target="_blank">Javascript SDK API</a>!