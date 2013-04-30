# Logging in with OAuth 2.0

Let's login user "Bruce Wayne" with password "imbatman"

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

You'll be given back the following in the response:

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

* You should save the `access_token` and `mac_key` somewhere you can refer to for the remainder of the session.  
* Similarly, because the access token only has one hour to live (as given by `expires_in`), it's recommended you save the expire time somewhere so you can check for the validity of the token.  e.g. `expireTime = (new Date()).getTime() + 3600 * 1000);`  However, this is not required.  It's simply so that your client can check to see if the OAuth tokens are still valid by checking the local time against the expire time.
* Also for convenience, it's recommended you save the logged in user locally for reference as well:  `theReturnedJSON['user']['username']`.  This is for convenience so that you can display who's logged in.

You can store the above into Local Storage, for instance.


# Making Requests with OAuth 2.0 Signature (After Logging In)

Make API request as you normally would (see REST API docs for StackMob functionality).  If the user is logged in, you'll want to include an extra header: `Authorization`.  Below, we'll go over how to generate the `Authorization` string.  Example of request to get all books.

    Request URL:
    http://api.stackmob.com/books

    Request Headers:
    Accept: application/vnd.stackmob+json; version=0
    X-StackMob-API-Key: {The Public Key}
    X-StackMob-User-Agent: { your user agent.  recommended format: My SDK v.0.5.5}
    Authorization:MAC id="vV6xEfVgQZv4ABJ6VZDHlQfCaqKgFZuN",ts="1343427512",nonce="n2468",mac="79js6rr3ynOCyssOHuGpGikfpvs="


# Generating the Authorization Header

StackMob follows the OAuth 2.0 spec for signing Authorization headers as depicted here:  [http://tools.ietf.org/html/draft-ietf-oauth-v2-http-mac-01](http://tools.ietf.org/html/draft-ietf-oauth-v2-http-mac-01)

StackMob uses *hmac-sha-1* and then base64 encodes the result along with some other items.  Below is a JS implementation of  

## StackMob OAuth 2.0 Implementation Examples (iOS/JS)

Here's an example of StackMob's JS SDK implementation of the signing: [https://gist.github.com/9515a7ecdbb5625b348b](https://gist.github.com/9515a7ecdbb5625b348b)

Here's an example of StackMob's iOS SDK implementation of the signing: [https://gist.github.com/3410085](https://gist.github.com/3410085)