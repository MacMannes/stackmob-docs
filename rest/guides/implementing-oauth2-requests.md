Making REST API Calls
===============================================

## Overview

StackMob has a REST API to expose almost all of its services.  If you're using the iOS, Android, or JavaScript SDKs, the SDKs are making HTTP REST API requests for you.

StackMob supports both OAuth 1.0 and OAuth 2.0 protocols in identifying your app/users.  We'll show you an introduction on making calls to StackMob with OAuth 2.0 in this guide, which is what our SDKs use.  (<a href="/rest-api/oauth1-and-oauth2-guide">Read how StackMob uses OAuth 1.0 and OAuth 2.0.</a>)

What if you wanted to make your own REST API calls from scratch?  What is a REST API call?  What would you need to set?  This is an introduction to making basic OAuth 2.0 REST API requests to StackMob.

## REST API Call Anatomy

Making a REST API call is really just making a HTTP request.  An HTTP request has the key components of:

* a URL
* an HTTP Verb, e.g, GET, POST, PUT, DELETE
* Request Headers
* Optional Request Body

### GET

Here's a typical example StackMob REST API request which we'll break down.  It fetches all users for your app from your development environment.  It's an OAuth 2.0 request.  Pass in your public key via the `X-StackMob-API-Key` request header.  You can find your public keys in <a href="https://dashboard.stackmob.com/settings" target="_blank">your application settings</a>.

```xml,5
HTTP Verb and URL:
GET http://api.stackmob.com/user

Request Headers:
X-StackMob-API-Key: dc0e228a-ccd3-4799-acd5-819f6c074ace
Accept: application/vnd.stackmob+json; version=0

Request Body:

*none*
```

<p class="alert">
	Each request has a <code>X-StackMob-API-Key</code> and your <a href="https://dashboard.stackmob.com/settings" target="_blank">public key</a> in the header to identify your app.
</p>

StackMob sees `/user` and by default routes it as a schema call.  We know it's your app because you've identified your app with your `X-StackMob-API-Key`, your application's development public key we gave you.  We also know whether you're hitting your development or production environment based on the `version` number you're passing up.  `0` is development, and anything `1` and above will be a specific API version you've rolled.

### POST

Let's create a user with the `POST` verb.

```xml
HTTP Verb and URL
POST http://api.stackmob.com/user

Request Headers:
X-StackMob-API-Key: dc0e228a-ccd3-4799-acd5-819f6c074ace
Accept: application/vnd.stackmob+json; version=0

Request Body:

{ username: 'joe', password: 'schmoe' }
```

## REST API Docs

So if REST API calls are just HTTP Verbs, URLs, headers, and request bodies, then anything that can make requests can take advantage of the StackMob REST API.  The full capabilities of the StackMob REST API are documented in the <a href="https://developer.stackmob.com/rest-api/api-docs" target="_blank">StackMob REST API Docs</a>

With it you can:

* access your StackMob datastore
* send push notifications
* run custom code

...and much more.

<a class="btn btn-info btn-large btn-flat btn-large-text margin-bottom-large margin-top-large" href="https://developer.stackmob.com/rest-api/api-docs" target="_blank">StackMob REST API Docs <i class="icon-chevron-right"></i></a>

## Authentication

Now that you can make basic OAuth 2.0 calls, you can now move onto logging in users with OAuth 2.0.  Logging in with OAuth 2.0 gives you the ability to identify which *user* is making a request, allowing you to restrict access to data by user with <a href="https://developer.stackmob.com/modules/acl/docs" target="_blank">StackMob's Access Controls</a>.

Here's how it works:

1.  Hit our login endpoint `accessToken` with your user's `username` and `password`
2.  Get unique credentials back from StackMob
3.  Use those credentials to generate a string `Authorization` header for each request
4.  StackMob will know which user is making the call!

Let's get to it.

### Login

Let's log in user "Bruce Wayne" with password "imbatman".  We assume that you've already created an instance of this `user` in your datastore.

Your REST API call will have the format of:

    Request URL:
    POST http://api.stackmob.com/user/accessToken

    Request Headers:
    Content-Type:application/x-www-form-urlencoded
    Accept: application/vnd.stackmob+json; version=0
    X-StackMob-API-Key: /* The Public Key*/
    X-StackMob-User-Agent: /* your user agent.  recommended format: My SDK version # */

    Request Body:
    username=Bruce%20Wayne&password=imbatman&token_type=mac

This is a log in request to StackMob.

<p class="alert">
Notice that the <code>Content-Type</code> is <code>application/x-www-form-urlencoded</code>.  Also, it's not just the <code>username</code> and <code>password</code> we're passing in the request body.  We're also passing <code>token_type=mac</code>.
</p>

Assuming that you've provided the correct username and password, you'll be given back the following in the response:

    {
      "access_token":"vV6xEfVgQZv4ABJ6VZDHlQfCaqKgFZuN",
      "mac_key":"okKXxMWOEhnM78Rie02ZjWjP7eQqpp6V",
      "mac_algorithm":"hmac-sha-1",
      "token_type":"mac",
      "expires_in":3600,
      "refresh_token":"nZSiH3L5K4febMlELguILucrWpjRud56",
      "stackmob":{
        "user":{"username":"Bruce Wayne"} 
      }
    }

You should save the `access_token` and `mac_key` somewhere you can refer to for the remainder of the session.

Similarly, because the access token only has one hour to live (as given by `expires_in`), it's recommended you save the expire time somewhere so you can check for the validity of the token.  e.g. `expireTime = (new Date()).getTime() + 3600 * 1000);` However, this is not required.  It's simply so that your client can check to see if the OAuth tokens are still valid by checking the local time against the expire time.

Also for convenience, it's recommended you save the logged in user locally for reference as well:  `theReturnedJSON['user']['username']`.  This is for convenience so that you can display who's logged in.

You can store the above into Local Storage, for instance.

### Making Requests with OAuth 2.0 Signature (After Logging In)

Make API request as you normally would (see REST API docs for StackMob functionality).  If the user is logged in, you'll want to include an extra header: `Authorization`.  Below, we'll go over how to generate the `Authorization` string.  Example of request to get all books.

```xml,8
Request URL:
http://api.stackmob.com/books

Request Headers:
Accept: application/vnd.stackmob+json; version=0
X-StackMob-API-Key: {The Public Key}
X-StackMob-User-Agent: { your user agent.  recommended format: My SDK v.0.5.5}
Authorization:MAC id="vV6xEfVgQZv4ABJ6VZDHlQfCaqKgFZuN",ts="1343427512",nonce="n2468",mac="79js6rr3ynOCyssOHuGpGikfpvs="
```


### Generating the Authorization Header

StackMob follows the OAuth 2.0 spec for signing Authorization headers as depicted here:  [http://tools.ietf.org/html/draft-ietf-oauth-v2-http-mac-01](http://tools.ietf.org/html/draft-ietf-oauth-v2-http-mac-01)

StackMob uses *hmac-sha-1* and then base64 encodes the result along with some other items.  Below is a JS implementation of  

### StackMob OAuth 2.0 Implementation Example

Here's an example of StackMob's JS SDK implementation of the signing.  We are trying to generate the value of `Authorization` in the request header:

```xml
Authorization:MAC id="vV6xEfVgQZv4ABJ6VZDHlQfCaqKgFZuN",ts="1343427512",nonce="n2468",mac="79js6rr3ynOCyssOHuGpGikfpvs="
```

For readability, an example in JavaScript is shown below.  StackMob has broken it down into three functions:

* getAuthHeader - this generates the string for Authorization
* generateMAC - called by getAuthHeader.  This sets the format of the string
* createBaseString - more algorithm shenanigans here!

You *do not* need to break it down into three functions of course.  Let's go through each of them.

#### getAuthHeader

We'll set the `Authorization` header with the value returned by this function.  Per the above, the value returned would be something like:

```xml
MAC id="vV6xEfVgQZv4ABJ6VZDHlQfCaqKgFZuN",ts="1343427512",nonce="n2468",mac="79js6rr3ynOCyssOHuGpGikfpvs="
```

We'll pass in the `httpVerb` (GET, POST, PUT, DELETE) and the `url` we're calling (including parameters).  Below, we'll get all users with `age > 21`.

Here we do some preparation by parsing out the `host` and `path`.

```js
//Assume httpVerb is GET
//Assume url is https://api.stackmob.com/user?age[gt]=21

function getAuthHeader(httpVerb, url){

  // https://api.stackmob.com
  var host = StackMob.getBaseURL(); 

  // /user?age[gt]=21
  var path = url.replace(new RegExp(host, 'g'), '/');

  // api.stackmob.com
  var hostWithPort = host.replace(
      new RegExp('^http://|^https://', 'g'), '').replace(new RegExp('/'), '');

  // The access token when you logged in
  var accessToken = StackMob.Storage.retrieve('oauth2.accessToken');

  // The mac key when you logged in
  var macKey = StackMob.Storage.retrieve('oauth2.macKey');

  if(StackMob.isOAuth2Mode() && accessToken && macKey) {

    // Generate the Header!
    var authHeader = generateMAC(httpVerb, accessToken, macKey, hostWithPort, path);
    return authHeader;
  }
}
```

#### generateMAC

`getAuthHeader` calls `generateMAC`.  Here we'll calculate a hash using HMAC SHA1 with the `MAC key` returned from when you logged in, and the `base string`, which is composed of the domain, port, and URL you are hitting.

```js
  // httpVerb is GET
  // accessToken is the access token you got from your login
  // MAC Key is the mac key you got from your login
  // hostWithPort is api.stackmob.com (or api.stackmob.com:443 if https)
  // url is /user?age[gt]=21

  function generateMAC(httpVerb, accessToken, macKey, hostWithPort, url) {
    var splitHost = hostWithPort.split(':');
    var hostNoPort = splitHost.length > 1 ? splitHost[0] : hostWithPort;

    // remember that if you're pointing at https, the port is 443!
    var port = splitHost.length > 1 ? splitHost[1] : 80;

    var ts = Math.round(new Date().getTime() / 1000);
    var nonce = "n" + Math.round(Math.random() * 10000);

    var base = createBaseString(ts, nonce, httpVerb, url, hostNoPort, port);

    var hash = CryptoJS.HmacSHA1(base, macKey);
    var mac = hash.toString(CryptoJS.enc.Base64);

    return 'MAC id="' + accessToken + '",ts="' + ts + '",nonce="' + nonce + '",mac="' + mac + '"';
  }
```

#### createBaseString

`createBaseString` is used when creating the HMAC SHA 1 hash.

```js
  function createBaseString(ts, nonce, method, uri, host, port) {
    var nl = '\u000A';
    return ts + nl + nonce + nl + method + nl + uri + nl + host + nl + port + nl + nl;
  }
```

Generate the `Authorization` header each time you make a request, because the timestamp is an important part of the signing.

With signed requests, you'll be able to login users and take advantage of the <a href="https://developer.stackmob.com/modules/cors/docs" target="_blank">Access Controls</a>.

## Help

Need help figuring out how to implement OAuth 2.0?  Before you get into anything too serious, maybe you can check to see if we have an SDK or community SDK ready for you already?  <a href="https://developer.stackmob.com/sdks">Check out all SDKs.</a>

If you think you'll need to write your own OAuth 2.0 client and are running into problems, perhaps a <a href="http://support.stackmob.com/entries/22511348-REST-API-login-documentation-incorrect" target="_blank" rel="nofollow">forum discussion on OAuth 2.0 implementation</a> covers the topic.

Still have questions?  Ask in our <a href="http://support.stackmob.com/forums/20022458-Questions" target="_blank" rel="nofollow">Community Forums</a>.