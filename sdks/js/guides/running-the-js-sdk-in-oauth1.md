Running the JavaScript SDK in OAuth 1.0 Mode
==========================================

## Overview

This guide will show you how to run the StackMob JavaScript SDK in OAuth v1.

## HTML

We'll be using <a href="https://github.com/bytespider/jsOAuth"> target="_blank" rel="nofollow">bytespider's jsOAuth OAuth 1.0 JS library</a>.

```html
<html>
<head>
  <--
  Include jsOAuth (OAuth 1 library)
  Find this at https://github.com/bytespider/jsOAuth
  https://github.com/downloads/bytespider/jsOAuth/jsOAuth-1.3.6.min.js
  -->
  <script type="text/javascript" src="library/jsOAuth-1.3.6.min.js"></script>
  <--! Include underscore.js, backbone.js, jquery, etc as you would normally with the JS SDK -->
  <script type="text/javascript" src="stackmob.js"></script>
  <script type="text/javascript" src="oauth-js-sdk-extensions.js"></script> <!-- see other file in gist. sets up the StackMob SDK to use jsOAuth for calls -->
</head>
<body>
...
</body>
</html>
```

## JavaScript

And let's define `oauth-js-sdk-extensions.js`:

```javascript

//oauth-js-sdk-extensions.js

_.extend(StackMob, {
  'ajax': function(model, params, method) {

    var oauth = new OAuth({
      consumerKey: 'XXXXXXXXXXXXXXXXXXXXXXXXXXX',
      consumerSecret: 'XXXXXXXXXXXXXXXXXXXXXXXXX'}
    );

    var success = params['success'];
    var defaultSuccess = function(response, options) {

    var result = response && response.text ? JSON.parse(response.text) : null;
    if(params["stackmob_count"] === true)
    result = response;

    StackMob.onsuccess(model, method, params, result, success);

  };

  var error = params['error'];
  var defaultError = function(response, options) {
    var responseText = response.responseText || response.text;
    StackMob.onerror(response, responseText, null, model, params, error);
  }

  oauth.request({
    method : params['type'],
    headers: params['headers'],
    url: params['url'],
    data: params['data'],
    success: defaultSuccess,
    failure: defaultError
  });

  //Use bytespider's JSOAuth library here to make the ajax call. JSOAuth does things using oauth 1.0

  //jsOAuth takes parameters to set what's in the body, header, etc, success, failure, and that is all inside "params", which is passed in.

  //make the ajax call with jsoauth
  }
);
```

You can now use the JS SDK as you normally would, but your requests are now signed as OAuth 1.0 requests, given you complete access to your data.
