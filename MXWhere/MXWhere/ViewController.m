//
//  ViewController.m
//  MXWhere
//
//  Created by sharexia on 9/4/23.
//

#import "ViewController.h"
#import "MXExternalSourceParseManager.h"

@interface ViewController ()
@property (strong) IBOutlet NSButton *testButton;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
}


- (IBAction)testButtonAction:(id)sender {
    if ([[MXExternalSourceParseManager manager] running]) {
        [[MXExternalSourceParseManager manager] stop];
    } else {
        [[MXExternalSourceParseManager manager] startWithPath:@"/Users/tvuanywhere/Downloads/11460544_da3-1-16.mp4"
                                                        index:0
                                                     complete:^(BOOL success) {
            
        }];
    }
}



@end
