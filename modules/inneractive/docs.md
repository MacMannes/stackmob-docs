inneractive
=====================================================

## Overview

inneractive is the industry’s leading mobile app monetization exchange with over 100 global and local ad partners as well as multiple monetization verticals. In addition to mobile display advertising, inneractive supports rich media ads, virtual currency, and hyper-local targeting. StackMob offers full inneractive integration in our iOS and Android SDKs.

## Adding inneractive to your StackMob app

### Sign up with inneractive

Create a new inneractive account at <a rel="nofollow" href="https://console.inner-active.com/iamp/stackmob/signup" target="_blank">http://console.inner-active.com/stackmob/signup</a>. After you verify your email address, you'll be sent another email with your password, at which point you can…

### Sign into your dashboard on StackMob

Sign into inneractive on StackMob at <a href="https://dashboard.stackmob.com/module/inneractive/settings" target="_blank">https://dashboard.stackmob.com/addon/inneractive/</a>. From there, you can create a new app under the Add App tab. Once you've created a new app, copy the inneractive app id (displayed on the Dashboard tab) so you can…

### Add inneractive to your app

If you're using the latest <a href="https://developer.stackmob.com/sdks" target="_blank">Android and iOS StackMob SDKs</a> you already have inneractive sdks ready to use! Just start making calls to inneractive using your inneractive app id like so:

<span class="tab inneractive" title="iOS SDK"/>
**iOS SDK**

<a rel="nofollow"  href="https://s3.amazonaws.com/static.stackmob.com/resources/addons/inneractive/inneractive-sdk-ios.pdf">Inneractive iOS Docs</a>

```objc
[InneractiveAd DisplayAd:@"iOS_Test" withType:IaAdType_Banner withRoot:self.window withReload:120];
```
<span class="tab"/>

<span class="tab inneractive" title="Android SDK legacy"/>
**Android SDK**

<a rel="nofollow" href="https://s3.amazonaws.com/static.stackmob.com/resources/addons/inneractive/inneractive-sdk-android.pdf">Inneractive Android Docs</a>

```java
InneractiveAd.displayAd(this.getApplicationContext(), (ViewGroup) findViewById(R.id.linear), "StackMob_StackMobTest_Android", IaAdType.Banner, 120);
```
<span class="tab"/>


For Android, make sure you include the <a rel="nofollow" href="http://developer.android.com/sdk/compatibility-library.html">Android support jar</a> in your project to use inneractive.