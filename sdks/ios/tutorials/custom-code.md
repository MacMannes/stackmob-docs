Custom Code
===========

## Overview

Just want the full project? <a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/custom-code-request.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>

<h3>Objective</h3>

Perform a Custom Code request which returns the message hello world.

<h3>Experience Level</h3>
Beginner

<h3>Estimated time to complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* XCode 4.x and greater

* iOS SDK 5 and greater

* [Custom Code examples JAR uploaded to StackMob](https://developer.stackmob.com/tutorials/customcode/Build-and-Upload-Custom-Code-Example)

* [Download Base Xcode Project](https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/base-project.zip)

<h3>Have you read through the <a href="http://stackmob.github.com/stackmob-ios-sdk/#coding_practices" target="_blank">StackMob <—> Core Data Coding Practices</a>?</h3>

There are a few coding practices to adhere to as well as general things to keep in mind when using StackMob with Core Data. This allows StackMob to seamlessly translate to and from the language that Core Data speaks. Make sure to familiarize yourself with these practices, as you'll be using them often.

<h2>Build and Upload your Custom Code</h2> 
You'll need to build and upload a JAR based on the StackMob Custom Code examples.

We've created a separate tutorial called <a href="https://developer.stackmob.com/tutorials/customcode/Build-and-Upload-Custom-Code-Example">Build and Upload Custom Code Example</a>. If you haven't done this step before, complete that tutorial before continuing.


<h2>Open the Base Xcode Project</h2>

We’ve created an Xcode project for you as a starting place for this tutorial.  It has StackMob imported and the basic plumbing for Core Data.  This allows you to focus on the objective of this tutorial.

For more information on what's inside of the project, see <a href="https://developer.stackmob.com/tutorials/ios/Base-Xcode-Project-for-Tutorials" target="_blank">Base Xcode Project for Tutorials</a>.

Unzip the Base Project and open **base-project.xcodeproj**.

<h2>Add your Public Key</h2>
Go to <a href="https://dashboard.stackmob.com/settings" target="_blank">Manage App Info</a> in the StackMob Dashboard and copy the **Development Public Key** and paste it  into the **AppDelegate.m** file where is says **YOUR\_PUBLIC\_KEY** in the method:

```obj-c,4
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
    self.client = [[SMClient alloc] initWithAPIVersion:@"0" publicKey:@"YOUR_PUBLIC_KEY"];
    self.coreDataStore = [self.client coreDataStoreWithManagedObjectModel:self.managedObjectModel];
    return YES;
}
```

<h2>Create Your View</h2> 

Select the storyboard file and drag and drop a  **button** to your view.  Double-click the button to change its label to **Hello World**
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/tutorial/customcode/custom_code_request-01.png">
<br>
<br>
Hold down the **Option** key and click on **ViewController.h** (the ViewController.h file will open next to your storyboard.)

<br/>
Hold the **Control** key and click on the **button** and drag to your **ViewController.h** file.
<br><br>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/tutorial/customcode/custom_code_request-02.png">
<br>  
<br/>
In the menu, set the Connection to **Action** and enter the name **sendHelloWorld**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/tutorial/customcode/custom_code_request-03.png">
<br />
<br />

<h2>Open ViewController.h</h2>

The ViewController.h file should look like the following:

```obj-c
#import <UIKit/UIKit.h>

@interface ViewController : UIViewController

@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;
- (IBAction)sendHelloWorld:(id)sender;

@end
```

<h2>Open ViewController.m</h2>

Add the following **highlighted** code to your ViewController.m file:

ViewController.m

```obj-c,39-46
#import "ViewController.h"
#import "AppDelegate.h"
#import "StackMob.h"

@interface ViewController ()

@end

@implementation ViewController

@synthesize managedObjectContext = _managedObjectContext;

- (AppDelegate *)appDelegate {
    return (AppDelegate *)[[UIApplication sharedApplication] delegate];
}


- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    self.managedObjectContext = [[self.appDelegate coreDataStore] contextForCurrentThread];
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}

- (IBAction)sendHelloWorld:(id)sender {
    
    SMCustomCodeRequest *request = [[SMCustomCodeRequest alloc]
                                    initGetRequestWithMethod:@"hello_world"];
        
    [[[SMClient defaultClient] dataStore] performCustomCodeRequest:request onSuccess:^(NSURLRequest *request, NSHTTPURLResponse *response, id JSON) {
        NSLog(@"Success: %@",JSON);
    } onFailure:^(NSURLRequest *request, NSHTTPURLResponse *response, NSError *error, id JSON){
        NSLog(@"Failure: %@",error);
    }];

}
@end

```

<h2>Build and Run!</h2>

Run your project, click the Hello World button.  You'll see the message appear in the Xcode output console.

iOS Custom Code requests also support the POST, PUT, and DELETE HTTP method verbs, as well as attaching query string parameters for your method to use. Check out the <a href="http://stackmob.github.com/stackmob-ios-sdk/Classes/SMCustomCodeRequest.html" target="_blank">Custom Code Request Class</a> in the iOS SDK Reference for all the available methods. 

Congratulations on completing this tutorial!

<a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/custom-code-request.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>
