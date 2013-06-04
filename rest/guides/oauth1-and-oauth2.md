# StackMob and OAuth

StackMob has two ways of authorizing API calls to our servers:  OAuth 1.0 and OAuth 2.0.  Our Dashboard and SDKs use OAuth and take care of the OAuth implementation details for you.  OAuth 1.0 and 2.0 make use of the <a href="https://dashboard.stackmob.com/settings" target="_blank">application keys</a> so that our server knows what app to direct the requests to, and in OAuth 2.0's case, who the request is coming from.

Our iOS, Android, and JS SDKs use OAuth 2.0 to make calls to the REST API.  OAuth 2.0 only uses the <a href="https://dashboard.stackmob.com/settings" target="_blank">public key</a> that StackMob issues your application.

Our Dashboard Console and Dashboard Object Browser use OAuth 1.0, which uses both the <a href="https://dashboard.stackmob.com/settings" target="_blank">public and private keys</a> that StackMob issues your application.

Let's see how OAuth 1.0 and 2.0 are used at StackMob and what they mean.

If you're writing your own REST API client in another language, this document may help you decide on whether or not to use OAuth 1.0 or OAuth 2.0.

<h1 class="alwaysexpanded">OAuth 1.0</h1>

StackMob issues your application <a href="https://dashboard.stackmob.com/settings" target="_blank">public and private key pairs</a> - one pair for your development environment and another for your production environment.  OAuth 1.0 signs requests with both the public and private key to identify that the request is a valid one for your app. Your public and private keys are unique to your app.

<h2>What uses OAuth 1.0?</h2>

What uses OAuth 1.0 on StackMob?

* <a href="https://dashboard.stackmob.com/data/console" target="_blank">Dashboard Console</a>
* <a href="https://dashboard.stackmob.com/data/browser" target="_blank">Dashboard Object Browser</a>
* iOS and Android SDKs that are configured to use Push (we expect to move this to OAuth 2.0 in the future)

<h2>When is OAuth 1.0 used?</h2>

For StackMob, we assume that if you know the private key, you're the developer.  Hence if you use OAuth 1.0 (meaning you have the private key), you have privileged access to the API, and your calls bypass the <a href="http://developer.stackmob.com/tutorials/security/Access-Controls:-Schema-Permissions" target="_blank">logged-in access control rules</a> you've specified for your app.  As such, OAuth 1.0/private key access is generally used when you want full access to your data, which is why the Dashboard Console and Dashboard Object Browser use OAuth 1.0.

Example:
  
You're writing a PHP admin site for your enduser and want your user to have access to all the data, bypassing the Access Control restrictions.  You'd write an PHP client that implements OAuth 1.0 (a simple <a href="https://github.com/stackmob/stackmob-php-examples" target="_blank">example of PHP, OAuth 1.0, and StackMob</a>)


**The private key should NEVER be given out in public.**


<h1 class="alwaysexpanded">OAuth 2.0</h1>

OAuth 2.0 only uses the public key to connect to the REST API.  It doesn't need the private key StackMob issues you.

<h2>What uses OAuth 2.0?</h2>

What uses OAuth 2.0 on StackMob?

* iOS, Android, JS SDKs
* <a href="https://dashboard.stackmob.com/data/console" target="_blank">Dashboard Console</a>

<h2>OAuth 2.0 and (no) Private Key</h2>

The iOS, Android, and JS SDKs use OAuth 2.0 to authenticate API calls.  When you initialize your SDKs, you only provide your public key.  You don't use the private key that StackMob issues you.

When your users login with their username and password, they're each issued a personal private key which the SDK takes care of behind the scenes.  You don't have to deal with it at all.  That personal private key (which has an expiration date of 1 hour), is used to sign the request.  As a result, the server not only knows which application the request is for, but which user is making the call.

With OAuth 2.0, REST API calls will respect the <a href="http://developer.stackmob.com/tutorials/security/Access-Controls:-Schema-Permissions" target="_blank">Access Control rules</a> you've set, as we will know what user is logged in based on the token.

* Twitter and Facebook use OAuth 2.0 in their JS SDKs.

<h2>When is OAuth 2.0 used?</h2>

OAuth 2.0 is preferred when you want to login users and so that you can respect Access Controls.

Example:

You have a PHP app where users login.  Only they should be able to see and edit their `profile`, which you've represented as a schema.  You may want to implement a PHP OAuth 2.0 client so that you can login your users and make user-signed calls to the REST API.  This way, you can take advantage of the Access Control permissions you set for `profile` - for instance, setting `Read` to <a href="http://developer.stackmob.com/tutorials/security/Access-Controls:-Schema-Permissions#a-allow_to_object_owner" target="_blank">Allow Access to sm_owner (Object Owner)</a>.

You would also use OAuth 2.0 if you don't want to have your private key in the code.