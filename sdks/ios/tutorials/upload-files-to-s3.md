Upload Files to Amazon S3
=========================

## Overview

Just want the full project? <a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/upload-to-s3.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>

<h3>Objective</h3>

Use the Amazon S3 integration to persist binary data such as photos or files.

<h3>Experience Level</h3>
Beginner

<h3>Estimated time to complete</h3>
~5 minutes

<h3>Prerequisites</h3>

* XCode 4.x and greater

* iOS SDK 5 and greater

* [Download Base Xcode Project](https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/base-project.zip)

<h3>Have you read through the <a href="http://stackmob.github.com/stackmob-ios-sdk/#coding_practices" target="_blank">StackMob <—> Core Data Coding Practices</a>?</h3>

There are a few coding practices to adhere to as well as general things to keep in mind when using StackMob with Core Data. This allows StackMob to seamlessly translate to and from the language that Core Data speaks. Make sure to familiarize yourself with these practices, as you'll be using them often.

<h1>Let's get started!</h1>

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

<h2>Add a property to your model</h2> 
We've already setup a **Todo** entity in the base project.  Now let's add a new **photo** property.

Select the **mydatamodel.xcdatamodeld** file.  Click the plus sign under the **attributes** panel.  Give it the name **photo** with a data-type **string**.

<h2>Add a binary field to your schema</h2> 
You'll need to configure Amazon S3 and add a binary field to your Todo schema in the StackMob dashboard.

We've created a separate tutorial called <a href="https://developer.stackmob.com/tutorials/dashboard/Adding-a-Binary-Field-to-Schemas">Adding a Binary Field to Schemas</a>. If this is the first time you've setup a binary field on StackMob, follow that tutorial before continuing.


<h2>Create Your View</h2> 

Select the storyboard file and drag and drop a **button** to your view.  Double-click the button to change its label to **File Upload**
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/upload-to-s3/upload-to-s3-01.png">
<br/>
<br/>
Hold down the **Option** key and click on **ViewController.h** (the ViewController.h file will open next to your storyboard.)
<br/>
<br/>
Hold the **Control** key and click on the **button** and drag to your **ViewController.h** file.  
<br/>
In the menu, set the Connection to **Action** and enter the name **fileUpload**.
<br/>
<br/>
<img src="https://s3.amazonaws.com/static.stackmob.com/images/ios/tutorials/upload-to-s3/upload-to-s3-02.png">
<br />
<br />

<h2>Open ViewController.h</h2>


Make the ViewController.h file an **Image Picker Controller Delegate** and 
a **Navigation Controller Delegate** :


```obj-c,2,4
#import <UIKit/UIKit.h>
#import "StackMob.h"

@interface ViewController : UIViewController <UIImagePickerControllerDelegate, UINavigationControllerDelegate>

@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;

- (IBAction)fileUpload:(id)sender;

@end
```

<h2>Open ViewController.m</h2>

Add the following **highlighted** code to your ViewController.m file:

ViewController.m

```obj-c,41-53,56-66,69-97

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

/*
 Add the UIImagePickerController to the fileUpload method.
 */
- (IBAction)fileUpload:(id)sender {
    
    UIImagePickerController *imagePicker = [[UIImagePickerController alloc]init];
    
    if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera]) {
        
        [imagePicker setSourceType:UIImagePickerControllerSourceTypeCamera];
    } else {
        
        [imagePicker setSourceType:UIImagePickerControllerSourceTypePhotoLibrary];
    }
    
    [imagePicker setDelegate:self];
    
    [self presentViewController:imagePicker animated:YES completion:nil];
}

/*
 Add the did finish picker method to handler the image returned from the library.
 */
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    UIImage *image = [info objectForKey:UIImagePickerControllerOriginalImage];
    
    [self insertNewObject: image];
    
    [self dismissViewControllerAnimated:YES completion:nil];    
}


/*
 Take the image data and convert it to binary string data for saving on StackMob.
 */
- (void)insertNewObject:(id)image
{
    
    NSEntityDescription *entity = [NSEntityDescription entityForName:@"Todo" inManagedObjectContext:self.managedObjectContext];
    
    NSManagedObject *newManagedObject = [NSEntityDescription insertNewObjectForEntityForName:[entity name] inManagedObjectContext:self.managedObjectContext];
    
    // Create the NSData representation of the UIImage object sent as an argument.
    NSData *imageData = UIImageJPEGRepresentation(image, 0.7);
    
    // Convert the binary data to string to save on Amazon S3
    NSString *picData = [SMBinaryDataConversion stringForBinaryData:imageData name:@"someImage.jpg" contentType:@"image/jpg"];
    
    [newManagedObject setValue:picData forKey:@"photo"];
    [newManagedObject setValue:[NSString stringWithFormat:@"Todo with Image"] forKey:@"title"];
    [newManagedObject setValue:[newManagedObject assignObjectId] forKey:[newManagedObject primaryKeyField]];
    
    // Save the context.
    [self.managedObjectContext saveOnSuccess:^{
        [self.managedObjectContext refreshObject:newManagedObject mergeChanges:YES];
        NSLog(@"Saved object with photo!");
    } onFailure:^(NSError *error) {
        NSLog(@"Error saving: %@", error);
    }];
    
}

@end
```

<h2>Build and Run!</h2>

**Note: You'll need a photo in your simulator library to upload.  If you don't have any photos, you can open the web browser in your simulator, visit a website and click and hold to save an image into your simulator library.**

Run your project and click the upload file button. Watch the debug output for a response from StackMob when the file upload is complete.

You can view your results in the <a href="https://dashboard.stackmob.com/data/browser/todo" target="_blank">StackMob Object Browser</a>. 


Congratulations on completing this tutorial!

<a href="https://s3.amazonaws.com/static.stackmob.com/tutorial-source-code/ios/upload-to-s3.zip" class="gs-button green-text"><i class="icon-download-alt icon-medium"></i> Download Source Code</a>