# PhoneGap (Cordova) and StackMob

You can use the StackMob JS SDK with PhoneGap.

There are two things you'll need to do.

# 1 of 2. Include JS Dependencies

To use the StackMob JS SDK with PhoneGap, simply include the StackMob JS SDK and its dependencies along with the rest of your project.  Assuming you have saved local copies of the files below, add them to your PhoneGap app's `<head>` tags.  You can find the current SDK versions on the <a href="https://developer.stackmob.com/js-sdk/configure" target="_blank">StackMob JS SDK Configure page</a> under "Using your own existing projects".

```xml
<head>
	<script type="text/javascript" src="jquery.min.js"></script>
	<!-- Replace with the latest StackMob JS SDK version. -->
	<script type="text/javascript" src="stackmob-js-x.y.z-bundled-min.js"></script>
</head>
```

Here are download links for the above files. 

# 2 of 2. Whitelist External Domains

Whitelist the StackMob API servers so that your PhoneGap app can make calls to your REST API.  Below are the settings for iOS and Android for convenience.

Visit <a href="http://docs.phonegap.com/en/1.9.0/guide_whitelist_index.md.html" target="_blank">PhoneGap's docs on how to allow External Hosts</a>

## iOS PhoneGap

Look for `Cordova.plist` or `PhoneGap.plist` in the `Supporting Files` folder.  Add the following:

```javascript
api.stackmob.com
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
  <access origin="api.stackmob.com" subdomains="true"/>
  ....
</cordova>
```

# Making API Calls

After you've setup the above, use the JS SDK as you normally would!  So, get started by reading the <a href="https://developer.stackmob.com/js-sdk/developer-guide" target="_blank">JS SDK Developer Guide</a> and the <a href="https://developer.stackmob.com/js-sdk/api-docs" target="_blank">Javascript SDK API</a>!
