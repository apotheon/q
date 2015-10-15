require 'rails_helper'

feature 'User Profile' do
  context 'a logged in user' do
    let(:password) { 'password' }
    let(:user) do
      create :user,
      email: 'bobby@example.com',
      name: 'bobby',
      password: password
    end

    before do
      visit user_registration_path
      fill_in 'Email', with: user.email
      fill_in 'Name', with: user.name
      fill_in 'Password', with: password
      fill_in 'Confirm Password', with: password
      click_on 'Submit'
    end

    scenario 'visits the user profile' do
      expect(page).to have_content user.name
      expect(page).to have_content 'Profile'
    end
  end
end
